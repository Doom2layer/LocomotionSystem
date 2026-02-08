// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeBase.h"
#include "Unarmed_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API AUnarmed_Weapon : public AMeleeBase
{
	GENERATED_BODY()

	AUnarmed_Weapon();
	
public:
	virtual void BeginPlay() override;
};
