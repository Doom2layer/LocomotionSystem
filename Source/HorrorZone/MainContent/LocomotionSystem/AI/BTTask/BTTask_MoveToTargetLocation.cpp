// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/AI/BTTask/BTTask_MoveToTargetLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToTargetLocation::UBTTask_MoveToTargetLocation()
{
	NodeName = "Move To Target Location";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

uint16 UBTTask_MoveToTargetLocation::GetInstanceMemorySize() const
{
	return sizeof(FBTMoveToLocationMemory);
}

EBTNodeResult::Type UBTTask_MoveToTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTMoveToLocationMemory* MyMemory = reinterpret_cast<FBTMoveToLocationMemory*>(NodeMemory);
	MyMemory->OwnerComp = &OwnerComp;
	MyMemory->MoveRequestID = FAIRequestID::InvalidRequest;
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	FString PawnName = Pawn ? Pawn->GetName() : TEXT("Unknown");

	FAIMoveRequest MoveRequest;
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	if (bUseTargetActor)
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
		if (!TargetActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("BTTask_MoveToTargetLocation [%s]: Target actor is null"), *PawnName);
			return EBTNodeResult::Failed;
		}
		UE_LOG(LogTemp, Log, TEXT("BTTask_MoveToTargetLocation [%s]: Moving to target actor %s"), *PawnName, *TargetActor->GetName());
		MoveRequest.SetGoalActor(TargetActor);
	}
	else
	{
		FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationKey.SelectedKeyName);
		if (TargetLocation.IsZero())
		{
			UE_LOG(LogTemp, Warning, TEXT("BTTask_MoveToTargetLocation [%s]: Target location is zero"), *PawnName);
			return EBTNodeResult::Failed;
		}
		UE_LOG(LogTemp, Log, TEXT("BTTask_MoveToTargetLocation [%s]: Moving to location %s"), *PawnName, *TargetLocation.ToString());
		MoveRequest.SetGoalLocation(TargetLocation);
	}

	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult MoveResult = AIController->MoveTo(MoveRequest, &NavPath);
	MyMemory->MoveRequestID = MoveResult.MoveId;

	UE_LOG(LogTemp, Log, TEXT("BTTask_MoveToTargetLocation [%s]: MoveTo result code: %d"), 
		*PawnName, static_cast<int32>(MoveResult.Code));

	if (MoveResult.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		// Register observer using a lambda to capture NodeMemory
		UPathFollowingComponent* PathFollowingComp = AIController->GetPathFollowingComponent();
		if (PathFollowingComp)
		{
			MyMemory->DelegateHandle = PathFollowingComp->OnRequestFinished.AddUObject(this, &UBTTask_MoveToTargetLocation::OnMoveCompleted, NodeMemory);
		}
		
		// Movement request was accepted, wait for it to complete
		UE_LOG(LogTemp, Log, TEXT("BTTask_MoveToTargetLocation [%s]: Movement started, waiting for completion"), *PawnName);
		return EBTNodeResult::InProgress;
	}

	UE_LOG(LogTemp, Warning, TEXT("BTTask_MoveToTargetLocation [%s]: MoveTo failed with result %d"), *PawnName, static_cast<int32>(MoveResult.Code));
	return EBTNodeResult::Failed;
}

void UBTTask_MoveToTargetLocation::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FBTMoveToLocationMemory* MyMemory = reinterpret_cast<FBTMoveToLocationMemory*>(NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		UPathFollowingComponent* PathFollowingComp = AIController->GetPathFollowingComponent();
		if (PathFollowingComp && MyMemory->DelegateHandle.IsValid())
		{
			PathFollowingComp->OnRequestFinished.Remove(MyMemory->DelegateHandle);
		}
		
		if (PathFollowingComp && TaskResult != EBTNodeResult::Succeeded)
		{
			PathFollowingComp->AbortMove(*this, FPathFollowingResultFlags::OwnerFinished, MyMemory->MoveRequestID);
		}
	}

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

EBTNodeResult::Type UBTTask_MoveToTargetLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTMoveToLocationMemory* MyMemory = reinterpret_cast<FBTMoveToLocationMemory*>(NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		UPathFollowingComponent* PathFollowingComp = AIController->GetPathFollowingComponent();
		if (PathFollowingComp && MyMemory->DelegateHandle.IsValid())
		{
			PathFollowingComp->OnRequestFinished.Remove(MyMemory->DelegateHandle);
		}
		
		if (PathFollowingComp)
		{
			PathFollowingComp->AbortMove(*this, FPathFollowingResultFlags::OwnerFinished, MyMemory->MoveRequestID);
		}
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_MoveToTargetLocation::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, uint8* NodeMemory)
{
	FBTMoveToLocationMemory* MyMemory = reinterpret_cast<FBTMoveToLocationMemory*>(NodeMemory);
	
	// Check if this callback is for our move request
	if (RequestID != MyMemory->MoveRequestID)
	{
		return;
	}
	
	if (!MyMemory->OwnerComp.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_MoveToTargetLocation: OwnerComp is invalid in OnMoveCompleted"));
		return;
	}

	UBehaviorTreeComponent* OwnerComp = MyMemory->OwnerComp.Get();
	AAIController* AIController = OwnerComp->GetAIOwner();
	APawn* Pawn = AIController ? AIController->GetPawn() : nullptr;
	FString PawnName = Pawn ? Pawn->GetName() : TEXT("Unknown");
	
	UE_LOG(LogTemp, Log, TEXT("BTTask_MoveToTargetLocation [%s]: OnMoveCompleted called with result %d"), 
		*PawnName, static_cast<int32>(Result.Code));

	if (Result.IsSuccess())
	{
		UE_LOG(LogTemp, Log, TEXT("BTTask_MoveToTargetLocation [%s]: Movement completed successfully"), *PawnName);
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_MoveToTargetLocation [%s]: Movement failed with code %d"), *PawnName, static_cast<int32>(Result.Code));
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
}

