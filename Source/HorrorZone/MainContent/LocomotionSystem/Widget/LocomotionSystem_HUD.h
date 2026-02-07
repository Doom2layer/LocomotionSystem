// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LocomotionSystem_HUD.generated.h"

class UUserInterfaceSystem;
/**
 * 
 */
UCLASS()
class HORRORZONE_API ALocomotionSystem_HUD : public AHUD
{
	GENERATED_BODY()

	ALocomotionSystem_HUD();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Interface")
	UUserInterfaceSystem* UserInterfaceSystem;
	
};
