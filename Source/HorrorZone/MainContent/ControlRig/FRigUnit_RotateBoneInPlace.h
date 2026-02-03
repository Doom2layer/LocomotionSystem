// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/RigUnit.h"
#include "FRigUnit_RotateBoneInPlace.generated.h"

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Rotate Bone In Place", Category = "Custom", Keywords = "Rotate,Transform,Bone"))
struct HORRORZONE_API FRigUnit_RotateBoneInPlace : public FRigUnitMutable
{
	GENERATED_BODY()

	FRigUnit_RotateBoneInPlace() : Bone(NAME_None), AmountToRotate(FQuat::Identity), CachedBone() {}

	RIGVM_METHOD()
	virtual void Execute() override;
	 
	UPROPERTY(meta = (Input))
	FName Bone;

	UPROPERTY(meta = (Input))
	FQuat AmountToRotate;

	UPROPERTY(transient)
	FCachedRigElement CachedBone;
};
