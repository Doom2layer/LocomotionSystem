// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSystem.generated.h"

class AWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORZONE_API UWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UseWeapon(int Slot);
	
	void IncrementSlot();

	void DecrementSlot();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initialize();

	void AddWeapon(TObjectPtr<AWeaponBase> Weapon);
	
	void EquipUnequipWeapon(const TObjectPtr<AWeaponBase> PreviousWeaponClass, const TObjectPtr<AWeaponBase> NewWeaponClass);
	
	TObjectPtr<AWeaponBase> GetCurrentSlot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	int CurrentSlot;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	int DefaultWeaponSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	TArray<TSubclassOf<AWeaponBase>> DefaultWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	TArray<TObjectPtr<AWeaponBase>> WeaponSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	AActor* Owner;
};
