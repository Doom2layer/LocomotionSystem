// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETeam : uint8
{
	Red UMETA(DisplayName = "RED"),
	Blue UMETA(DisplayName = "BLUE"),
	Green UMETA(DisplayName = "GREEN")
};

UENUM(BlueprintType)
enum class ETeamAffiliation : uint8
{
	Enemy UMETA(DisplayName = "Enemy"),
	Friendly UMETA(DisplayName = "Friendly"),
	Neutral UMETA(DisplayName = "Neutral")
};