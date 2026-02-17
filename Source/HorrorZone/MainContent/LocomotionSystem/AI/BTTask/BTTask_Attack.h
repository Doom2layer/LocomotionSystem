// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"

class UWeaponSystem;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void FinishTask();
	
	void StopFiring();
	
	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float StopDuration = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float FireDuration = 0.5f;
	
	FTimerHandle FireTimerHandle;
	
	FTimerHandle StopTimerHandle;
	
	UPROPERTY()
	UBehaviorTreeComponent *CachedOwnerComp;
	
	uint8* CachedNodeMemory;
	
	UPROPERTY()
	TObjectPtr<UWeaponSystem> WeaponSystem;
	
	UPROPERTY()
	TObjectPtr<APawn> Pawn;
};
