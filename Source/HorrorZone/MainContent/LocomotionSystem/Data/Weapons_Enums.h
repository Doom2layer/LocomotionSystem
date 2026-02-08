// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeapon : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Pistol UMETA(DisplayName = "Pistol"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Rifle UMETA(DisplayName = "Rifle"),
	BaseballBat UMETA(DisplayName = "Baseball Bat"),
	Sword UMETA(DisplayName = "Sword")
};
