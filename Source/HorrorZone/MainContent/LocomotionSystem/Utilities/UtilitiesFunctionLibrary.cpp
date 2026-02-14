// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "MainContent/LocomotionSystem/Character/LocomotionSystem_CharacterBase.h"
#include "MainContent/LocomotionSystem/Components/Helper/MontageHelper.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/LocomotionSystem.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"

UUserInterfaceSystem* UUtilitiesFunctionLibrary::GetUserInterfaceSystem(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World)
	{
		return nullptr;
	}

	const APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		return nullptr;
	}

	AHUD* HUD = PC->GetHUD();
	if (!HUD)
	{
		return nullptr;
	}

	if (UUserInterfaceSystem* System = HUD->GetComponentByClass<UUserInterfaceSystem>()) return System;
	return nullptr;
}

ULocomotionSystem* UUtilitiesFunctionLibrary::GetLocomotionSystem(AActor* Owner)
{
	if (Owner)
	{
		if (ULocomotionSystem* System = Owner->GetComponentByClass<ULocomotionSystem>()) return System;
	}
	return nullptr;
}

UMontageHelper* UUtilitiesFunctionLibrary::GetMontageHelper(ALocomotionSystem_CharacterBase* Owner)
{
	if (Owner)
	{
		if (UMontageHelper* Helper = Owner->GetComponentByClass<UMontageHelper>()) return Helper;
	}
	return nullptr;
}

UWeaponSystem* UUtilitiesFunctionLibrary::GetWeaponSystem(AActor* Owner)
{
	if (Owner)
	{
		if (UWeaponSystem* System = Owner->GetComponentByClass<UWeaponSystem>()) return System;
	}
	return nullptr;
}


ACharacter* UUtilitiesFunctionLibrary::GetCharacter(AActor* Actor)
{
	if (Actor) return Cast<ACharacter>(Actor);
	return nullptr;
}

APlayerCameraManager* UUtilitiesFunctionLibrary::GetCameraManager(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World)
	{
		return nullptr;
	}

	const APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		return nullptr;
	}

	return PC->PlayerCameraManager;
}