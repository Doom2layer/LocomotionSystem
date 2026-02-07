// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"
#include "Unarmed_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API AUnarmed_Weapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
