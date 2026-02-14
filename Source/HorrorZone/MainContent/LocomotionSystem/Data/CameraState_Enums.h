// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECameraState : uint8
{
	MediumCamera UMETA(DisplayName = "Medium Camera (Default Third Person)"),
	CombatCloseCamera UMETA(DisplayName = "Close Camera (Combat Mode)"),
	AimingCloseCamera UMETA(DisplayName = "Close Camera (Aiming Mode)"),
	FarCamera UMETA(DisplayName = "Far Camera (Exploration/Vehicle Mode)"),
	SprintCamera UMETA(DisplayName = "Sprint Camera"),
	DeathCamera UMETA(DisplayName = "Death Camera"),
};