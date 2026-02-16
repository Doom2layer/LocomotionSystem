// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Character/LocomotionSystem_CharacterBase.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/ActorProfileSystem.h"
#include "UtilitiesFunctionLibrary.generated.h"

class UWeaponSystem;
class ALocomotionSystem_CharacterBase;
class UMontageHelper;
class ULocomotionSystem;
class UUserInterfaceSystem;
class UActorProfileSystem;

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
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities", meta=(WorldContext="WorldContextObject"))
	static APlayerCameraManager* GetCameraManager(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static ULocomotionSystem* GetLocomotionSystem(AActor* Owner);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static UMontageHelper* GetMontageHelper(ALocomotionSystem_CharacterBase* Owner);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static UWeaponSystem* GetWeaponSystem(AActor* Owner);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static ACharacter* GetCharacter(AActor* Actor);	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utilities")
	static UActorProfileSystem* GetActorProfileSystem(AActor* Owner);
	
	
	
	template<typename T>
	static bool IsPlayer(T* Actor)
	{
		if (Actor)
		{
			if (ALocomotionSystem_CharacterBase* Character = Cast<ALocomotionSystem_CharacterBase>(Actor))
			{
				return Character->IsPlayerControlled();
			}
		}
		return false;
	}
	
		template<typename T>
		static bool IsNPC(T* Actor, FName Tag = "NPC")
		{
			if (Actor)
			{
				if (ALocomotionSystem_CharacterBase* Character = Cast<ALocomotionSystem_CharacterBase>(Actor))
				{
					if (UActorProfileSystem* System = UUtilitiesFunctionLibrary::GetActorProfileSystem(Actor))
					{
						return System->GetTags().Contains(Tag) && !Character->IsPlayerControlled();
					}
				}
			}
			return false;
		}
};
