// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAIBehaviour : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Roam UMETA(DisplayName = "Roam"),
	Attack UMETA(DisplayName = "Attack"),
};