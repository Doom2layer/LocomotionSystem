// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/AI/BTTask/BTTask_Attack.h"

#include "AIController.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	Pawn = AIController ? AIController->GetPawn() : nullptr;
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_Attack: Pawn is null"));
		return EBTNodeResult::Failed;
	}

	WeaponSystem = UUtilitiesFunctionLibrary::GetWeaponSystem(Pawn);
	if (!WeaponSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_Attack: WeaponSystem is null"));
		return EBTNodeResult::Failed;
	}

	AWeaponBase* CurrentWeapon = WeaponSystem->GetCurrentSlot();
	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_Attack: No weapon equipped"));
		return EBTNodeResult::Failed;
	}

	CurrentWeapon->Fire(true);

	// Shorter burst duration so AI can re-evaluate targets more frequently

	Pawn->GetWorldTimerManager().SetTimer(FireTimerHandle, this, &UBTTask_Attack::StopFiring, FMath::FRandRange(FireDuration, 1.f), false);

	CachedOwnerComp = &OwnerComp;
	CachedNodeMemory = NodeMemory;

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::StopFiring()
{
	if (WeaponSystem)
	{
		AWeaponBase* CurrentWeapon = WeaponSystem->GetCurrentSlot();
		if (CurrentWeapon)
		{
			CurrentWeapon->Fire(false);
		}
	}

	if (Pawn)
	{
		Pawn->GetWorldTimerManager().SetTimer(StopTimerHandle, this, &UBTTask_Attack::FinishTask, FMath::FRandRange(StopDuration, 0.5f), false);
	}
}


void UBTTask_Attack::FinishTask()
{
	if (CachedOwnerComp)
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// Stop firing when task finishes
	if (WeaponSystem)
	{
		AWeaponBase* CurrentWeapon = WeaponSystem->GetCurrentSlot();
		if (CurrentWeapon)
		{
			CurrentWeapon->Fire(false);
		}
	}
	
	// Clear timers
	if (Pawn)
	{
		Pawn->GetWorldTimerManager().ClearTimer(FireTimerHandle);
		Pawn->GetWorldTimerManager().ClearTimer(StopTimerHandle);
	}
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}