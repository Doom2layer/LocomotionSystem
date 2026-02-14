// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LocomotionInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class ULocomotionInterface : public UInterface
{
	GENERATED_BODY()
};

class ILocomotionInterface
{
	GENERATED_BODY()

public:
	virtual void Sprint(bool bIsPressed);
};
