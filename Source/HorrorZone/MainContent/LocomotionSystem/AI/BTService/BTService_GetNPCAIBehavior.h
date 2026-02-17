// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_GetNPCAIBehavior.generated.h"

class UNPCSystem;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UBTService_GetNPCAIBehavior : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_GetNPCAIBehavior();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY()
	TObjectPtr<UNPCSystem> NPCSystem;
	
	UPROPERTY()
	TObjectPtr<APawn> Pawn;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AIBehaviourKey;
	
};
