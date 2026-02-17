// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_MoveToTargetLocation.generated.h"

// Node memory struct to store per-instance data
struct FBTMoveToLocationMemory
{
	TWeakObjectPtr<UBehaviorTreeComponent> OwnerComp;
	FAIRequestID MoveRequestID;
	FDelegateHandle DelegateHandle;
	
	FBTMoveToLocationMemory()
		: MoveRequestID(FAIRequestID::InvalidRequest)
	{
	}
};

/**
 * 
 */
UCLASS()
class HORRORZONE_API UBTTask_MoveToTargetLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToTargetLocation();
	
	virtual uint16 GetInstanceMemorySize() const override;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, uint8* NodeMemory);
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptanceRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bUseTargetActor = false;
};
