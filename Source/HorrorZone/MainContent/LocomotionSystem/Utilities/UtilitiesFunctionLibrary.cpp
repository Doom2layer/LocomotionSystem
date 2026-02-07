// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"

UUserInterfaceSystem* UUtilitiesFunctionLibrary::GetUserInterfaceSystem(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World)
	{
		return nullptr;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		return nullptr;
	}

	AHUD* HUD = PC->GetHUD();
	if (!HUD)
	{
		return nullptr;
	}

	return Cast<UUserInterfaceSystem>(HUD->GetComponentByClass(UUserInterfaceSystem::StaticClass()));
}