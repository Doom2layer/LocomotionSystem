// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponInterface.generated.h"
/**
* Empty class for reflection system visibility.
* Uses the UINTERFACE macro.
* Inherits from UInterface.
*/
UINTERFACE()
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void Fire(bool bIsPressed);

	virtual void MeleeAttack(FHitResult HitResult);

	virtual void Reload();

	virtual void Melee(bool bIsPressed);
};
