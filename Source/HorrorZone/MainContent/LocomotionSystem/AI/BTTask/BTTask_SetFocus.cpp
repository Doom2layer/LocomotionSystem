// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/AI/BTTask/BTTask_SetFocus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"

EBTNodeResult::Type UBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_SetFocus: AIController is null"));
		return EBTNodeResult::Failed;
	}

	Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_SetFocus: Pawn is null"));
		return EBTNodeResult::Failed;
	}
	
	AActor* NearestActor = nullptr;
	float Distance = 0.0f;
	GetNearestTargetActor(NearestActor, Distance);
	
	if (NearestActor)
	{
		// Target found - set focus and update blackboard
		AIController->SetFocus(NearestActor);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, NearestActor);
		UE_LOG(LogTemp, Log, TEXT("UBTTask_SetFocus: Target found - %s"), *NearestActor->GetName());
		return EBTNodeResult::Succeeded;
	}
	
	// No target found - clear focus and blackboard
	UE_LOG(LogTemp, Log, TEXT("UBTTask_SetFocus: No valid target found"));
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	OwnerComp.GetBlackboardComponent()->ClearValue(TargetActorKey.SelectedKeyName);
	
	// Important: Return Succeeded so Sequence 1 completes successfully
	// The decorators on Sequences 2 and 3 will check if TargetActor is set/not set
	return EBTNodeResult::Succeeded;
}

void UBTTask_SetFocus::GetNearestTargetActor(AActor*& InNearestActor, float& InDistance)
{
    TMap<AActor*, float> ActorDistanceMap;
    ActorDistanceMap.Empty();
    TArray<AActor*> OutActors;
    TArray<AActor*> IgnoreActors = {Pawn};
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    
    UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Pawn->GetActorLocation(), SphereRadius, ObjectTypes, nullptr, IgnoreActors, OutActors);
    
    UActorProfileSystem* MyProfileSystem = UUtilitiesFunctionLibrary::GetActorProfileSystem(Pawn);
    if (!MyProfileSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTTask_SetFocus: Pawn has no ActorProfileSystem"));
        return;
    }
    
    for (AActor* Actor : OutActors)
    {
        if (Actor && Actor != Pawn)
        {
            UActorProfileSystem* TargetProfileSystem = UUtilitiesFunctionLibrary::GetActorProfileSystem(Actor);
            if (!TargetProfileSystem)
            {
                continue; // Skip actors without profile system
            }
            
            if (MyProfileSystem->GetTeamAffiliation(Actor) == Affiliation && !MyProfileSystem->GetIsDead())
            {
                float L_Distance = FVector::Dist(Pawn->GetActorLocation(), Actor->GetActorLocation());
                ActorDistanceMap.Add(Actor, L_Distance);
            }
        }
    }

    // Extract all distance values
    TArray<float> Distances;
    ActorDistanceMap.GenerateValueArray(Distances);

    // Find minimum value and it's index
    float MinDistance = TNumericLimits<float>::Max();
    int32 MinIndex = INDEX_NONE;

    for (int32 i = 0; i < Distances.Num(); i++)
    {
        if (Distances[i] < MinDistance)
        {
            MinDistance = Distances[i];
            MinIndex = i;
        }
    }
    
    // Get the actor key by index from keys array
    if (MinIndex != INDEX_NONE)
    {
        TArray<AActor*> Keys;
        ActorDistanceMap.GenerateKeyArray(Keys);
        InNearestActor = Keys[MinIndex];
        InDistance = MinDistance;
    }
}
