// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LSAIController.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API ALSAIController : public AAIController
{
	GENERATED_BODY()

	ALSAIController();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSoftObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
	virtual void OnPossess(APawn* InPawn) override;
};
