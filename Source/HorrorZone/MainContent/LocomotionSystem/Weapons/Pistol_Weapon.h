// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"
#include "Pistol_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API APistol_Weapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
