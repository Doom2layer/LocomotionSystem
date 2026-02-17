// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MainContent/LocomotionSystem/Data/ActorProfiles_Enums.h"
#include "BTTask_SetFocus.generated.h"

enum class ETeamAffiliation : uint8;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UBTTask_SetFocus : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void GetNearestTargetActor(AActor*& InNearestActor, float& InDistance);
	
	UPROPERTY(EditAnywhere, Category="BTTask")
	ETeamAffiliation Affiliation = ETeamAffiliation::Enemy;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY()
	APawn* Pawn;
	
	UPROPERTY()
	float SphereRadius = 1000.0f;
};
