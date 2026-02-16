// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESenseRule : uint8
{
	ActiveOnlyIfPlayerInRange UMETA(DisplayName = "Active Only If Player In Range"),
	AlwaysActive UMETA(DisplayName = "Always Active"),
	AlwaysInactive UMETA(DisplayName = "Always Inactive")
};