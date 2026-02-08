// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilitiesFunctionLibrary.generated.h"

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
};
