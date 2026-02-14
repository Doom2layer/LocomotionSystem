// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/PlayerController/LocomotionSystem_PC.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "MainContent/LocomotionSystem/Camera/HZ_CameraManager.h"

ALocomotionSystem_PC::ALocomotionSystem_PC()
{
	PlayerCameraManagerClass = AHZ_CameraManager::StaticClass();	
}

void ALocomotionSystem_PC::BeginPlay()
{
	Super::BeginPlay();
}

void ALocomotionSystem_PC::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}
