// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilitiesFunctionLibrary.generated.h"

class UWeaponSystem;
class ALocomotionSystem_CharacterBase;
class UMontageHelper;
class ULocomotionSystem;
class UUserInterfaceSystem;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UUtilitiesFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities", meta=(WorldContext="WorldContextObject"))
	static UUserInterfaceSystem* GetUserInterfaceSystem(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static ULocomotionSystem* GetLocomotionSystem(AActor* Owner);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static UMontageHelper* GetMontageHelper(ALocomotionSystem_CharacterBase* Owner);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static UWeaponSystem* GetWeaponSystem(AActor* Owner);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static bool IsPlayer(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static ACharacter* GetCharacter(AActor* Actor);	
};
