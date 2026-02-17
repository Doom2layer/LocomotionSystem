// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/AI/Controller/LSAIController.h"

#include "BehaviorTree/BehaviorTree.h"

ALSAIController::ALSAIController()
{
}

void ALSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	 
	UBehaviorTree* BT = BehaviorTreeAsset.Get();
	if (!BT && BehaviorTreeAsset.IsValid())
	{
		BT = BehaviorTreeAsset.LoadSynchronous();
	}
	 
	if (BT)
	{
		RunBehaviorTree(BT);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController: BehaviorTreeAsset not set or failed to load!"));
	}
}