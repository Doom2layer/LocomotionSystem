// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Character/LocomotionSystem_NPCBase.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"

void ALocomotionSystem_NPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (WeaponSystem)
		{
			ToggleFire(true);
		}
	}, 0.2f, false);
}

void ALocomotionSystem_NPCBase::BeginPlay()
{
	Super::BeginPlay();
	WeaponSystem->UseWeapon(4);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->SetFocus(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}
