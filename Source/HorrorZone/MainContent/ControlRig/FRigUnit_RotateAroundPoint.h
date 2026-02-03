// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/RigUnit.h"
#include "FRigUnit_RotateAroundPoint.generated.h"

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Rotate Around Point", Category = "Custom", Keywords = "Rotate,Transform,Point"))
struct HORRORZONE_API FRigUnit_RotateAroundPoint : public FRigUnitMutable
{
	GENERATED_BODY()
	 
	FRigUnit_RotateAroundPoint()
	 : InputTransform(FTransform::Identity)
	 , PointToRotateAround(FVector::ZeroVector)
	 , AmountToRotate(FQuat::Identity)
	 , OutputTransform(FTransform::Identity) {}
	 
	RIGVM_METHOD()
	virtual void Execute() override;
	 
	UPROPERTY(meta = (Input))
	FTransform InputTransform;
	 
	UPROPERTY(meta = (Input))
	FVector PointToRotateAround;

	UPROPERTY(meta = (Input))
	FQuat AmountToRotate;

	UPROPERTY(meta = (Output))
	FTransform OutputTransform;
	
};
