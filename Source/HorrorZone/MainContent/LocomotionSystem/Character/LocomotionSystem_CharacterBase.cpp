// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Character/LocomotionSystem_CharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/LocomotionSystem.h"

// Sets default values
ALocomotionSystem_CharacterBase::ALocomotionSystem_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LocomotionSystem = CreateDefaultSubobject<ULocomotionSystem>(TEXT("Locomotion System"));
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

// Called when the game starts or when spawned
void ALocomotionSystem_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALocomotionSystem_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALocomotionSystem_CharacterBase::ToggleWalk(bool IsPressed)
{
	if (!LocomotionSystem) return;

	if (IsPressed)
	{
		LocomotionSystem->SetMovementType(EMovementType::Walk);
	}
	else
	{
		LocomotionSystem->SetMovementType(EMovementType::Jog);
	}
}

void ALocomotionSystem_CharacterBase::ToggleCrouch(bool IsPressed)
{
	if (!LocomotionSystem) return;

	if (IsPressed)
	{
		LocomotionSystem->SetMovementType(EMovementType::Crouch);
		Crouch();
	}
	else
	{
		LocomotionSystem->SetMovementType(EMovementType::Jog);
		UnCrouch();
	}
}

void ALocomotionSystem_CharacterBase::ToggleSprint(bool IsPressed)
{
	if (!LocomotionSystem) return;

	if (IsPressed)
	{
		LocomotionSystem->SetSprint(true);
	}
	else
	{
		LocomotionSystem->SetSprint(false);
	}
}

void ALocomotionSystem_CharacterBase::ToggleJump(bool IsPressed)
{
	if (!LocomotionSystem) return;

	if (IsPressed)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void ALocomotionSystem_CharacterBase::SwitchRotationMode(bool IsPressed)
{
	if (!LocomotionSystem) return;

	if (IsPressed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Switch Rotation"));
		ERotationMode NewMode = LocomotionSystem->GetRotationMode() == ERotationMode::ForwardFacing ? ERotationMode::Strafing : ERotationMode::ForwardFacing;
		LocomotionSystem->SetRotationMode(NewMode);
	}
}
