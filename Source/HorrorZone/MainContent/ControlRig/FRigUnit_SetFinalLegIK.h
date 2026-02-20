// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/RigUnit.h"
#include "FRigUnit_SetFinalLegIK.generated.h"

/**
 * 
 */
USTRUCT(meta=(DisplayName="Set Final Foot IK", Category="Custom | Foot IK", Keywords="Foot,IK,TwoBone", Varying))
struct FRigUnit_SetFinalLegIK : public FRigUnitMutable
{
	GENERATED_BODY()
	 
	// Input: array of foot bones to apply IK on
	UPROPERTY(meta = (Input))
	TArray<FRigElementKey> FootItems;
	
	UPROPERTY(meta = (Input))
	bool bPropagateToChildren;
	 
	RIGVM_METHOD()
	virtual void Execute() override;
};