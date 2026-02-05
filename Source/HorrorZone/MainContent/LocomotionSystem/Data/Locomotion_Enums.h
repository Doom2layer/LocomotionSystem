// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECardinalDirection : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	BlendOut UMETA(DisplayName = "Blend Out"),
	Hold UMETA(DisplayName = "Hold"),
	Accumulate UMETA(DisplayName = "Accumulate")
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	None UMETA(DisplayName = "None"),
	Walk UMETA(DisplayName = "Walk"),
	Jog UMETA(DisplayName = "Jog"),
	Crouch UMETA(DisplayName = "Crouch")
};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	Strafing UMETA(DisplayName = "Strafing"),
	ForwardFacing UMETA(DisplayName = "Forward Facing")
};

UENUM(BlueprintType)
enum class EAnimBlendType : uint8
{
	None UMETA(DisplayName = "None"),
	FullBody UMETA(DisplayName = "Full Body"),
	UpperBody UMETA(DisplayName = "Upper Body"),
	UpperBodyMask UMETA(DisplayName = "Upper Body Mask"),
	UpperBodyLowerBodySplitMask UMETA(DisplayName = "Upper Body Lower Body Split Mask")
};