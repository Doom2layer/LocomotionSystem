// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainContent/LocomotionSystem/Data/Weapon_Structs.h"
#include "MainContent/LocomotionSystem/Interfaces/InventoryItemInterface.h"
#include "MainContent/LocomotionSystem/Interfaces/WeaponInterface.h"
#include "WeaponBase.generated.h"

class UMontageHelper;
class ALocomotionSystem_CharacterBase;
struct FS_WeaponConfig;

UCLASS()
class HORRORZONE_API AWeaponBase : public AActor, public IInventoryItemInterface, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// Interface Implementation
	// InventoryItemInterface
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
	
	// WeaponInterface
	virtual void Fire(bool bIsPressed) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeHUD();

	void SetAnimSet();

	void AttachToCharacter();

	void SetWeaponMesh();

	void SetWeaponVisibility(bool bVisibile);

	void PlayWeaponEquipAnimMontage();

	void PlayWeaponUnequipAnimMontage();

	void PlayMontageOnOwner(UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection, bool ShouldStopAllMontages, FName ID);

	void PerformMeleeAttack();
	
	UFUNCTION()
	void OnMontageCompletedAtOwner(FName AnimNotify);

	UFUNCTION()
	void OnMontageBlendOutAtOwner(FName AnimNotify);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup)
	USceneComponent* RootSceneComponent;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup)
	FS_WeaponConfig WeaponConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup)
	FS_MeleeWeaponConfigs MeleeWeaponConfigs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	ALocomotionSystem_CharacterBase* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	UMontageHelper* MontageHelper;	
	
};
