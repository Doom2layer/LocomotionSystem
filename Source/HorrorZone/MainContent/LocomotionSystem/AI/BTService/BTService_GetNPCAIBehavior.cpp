// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/AI/BTService/BTService_GetNPCAIBehavior.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/NPCSystem.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"

UBTService_GetNPCAIBehavior::UBTService_GetNPCAIBehavior()
{
	bCreateNodeInstance = true;   // Enable one service instance per AI agent
	bNotifyTick = true;           // Ensure TickNode is called
}

void UBTService_GetNPCAIBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Get the pawn each tick (or cache it once it's valid)
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	APawn* CurrentPawn = AIController->GetPawn();
	if (!CurrentPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTService_GetNPCAIBehavior: AIController has no pawn yet"));
		return;
	}

	// Cache NPCSystem if we don't have it yet or pawn changed
	if (!NPCSystem || Pawn != CurrentPawn)
	{
		Pawn = CurrentPawn;
		NPCSystem = UUtilitiesFunctionLibrary::GetNPCSystem(Pawn);
	}

	if (NPCSystem)
	{
		EAIBehaviour CurrentBehavior = NPCSystem->GetAIBehaviour();
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIBehaviourKey.SelectedKeyName, static_cast<uint8>(CurrentBehavior));
	}
}