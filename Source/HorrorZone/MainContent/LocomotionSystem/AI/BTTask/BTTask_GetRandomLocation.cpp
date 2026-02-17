// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/AI/BTTask/BTTask_GetRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	NodeName = "Get Random Location";
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	bool bSuccess;
	FVector RandomLocation = GetRandomLocation(Pawn, bSuccess);
	if (bSuccess)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsVector(RandomLocationKey.SelectedKeyName, RandomLocation);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

FVector UBTTask_GetRandomLocation::GetRandomLocation(APawn* Pawn, bool& bOutReturnValue)
{
	if (!Pawn)
	{
		 bOutReturnValue = false;
		return FVector::ZeroVector;
	}

	FNavLocation RandomLocation;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
	if (NavSys && NavSys->GetRandomPointInNavigableRadius( Pawn->GetActorLocation(), Radius, RandomLocation))
	{
		bOutReturnValue = true;
		return RandomLocation.Location;
	}
	bOutReturnValue = false;
	return FVector::ZeroVector;
}
