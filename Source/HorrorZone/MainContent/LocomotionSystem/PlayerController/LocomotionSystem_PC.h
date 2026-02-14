// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LocomotionSystem_PC.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class HORRORZONE_API ALocomotionSystem_PC : public APlayerController
{
	GENERATED_BODY()
	ALocomotionSystem_PC();
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
};
