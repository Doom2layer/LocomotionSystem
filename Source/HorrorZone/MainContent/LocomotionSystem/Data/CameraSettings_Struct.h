// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraState_Enums.h"
#include "CameraSettings_Struct.generated.h"

enum class ECameraState : uint8;
/**
 * 
 */
USTRUCT()
struct FS_CameraSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetArmLength = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CameraFOV = 90.0f;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform CameraTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECameraState CameraState = ECameraState::MediumCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* TargetArmLengthCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* FOVCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* TransformCurve = nullptr;
};
