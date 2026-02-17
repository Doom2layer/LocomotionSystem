// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API UBTTask_GetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UBTTask_GetRandomLocation();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	FVector GetRandomLocation(APawn* Pawn, bool& bOutReturnValue);
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector RandomLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float Radius = 1000.0f;
};
