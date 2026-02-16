// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/Character/LocomotionSystem_CharacterBase.h"
#include "LocomotionSystem_NPCBase.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API ALocomotionSystem_NPCBase : public ALocomotionSystem_CharacterBase
{
	GENERATED_BODY()

	ALocomotionSystem_NPCBase();
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
