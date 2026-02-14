// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainContent/LocomotionSystem/Data/Weapon_Structs.h"
#include "MainContent/LocomotionSystem/Interfaces/InventoryItemInterface.h"
#include "MainContent/LocomotionSystem/Interfaces/LocomotionInterface.h"
#include "MainContent/LocomotionSystem/Interfaces/WeaponInterface.h"
#include "WeaponBase.generated.h"

enum class ERotationMode : uint8;
class ULocomotionSystem;
class UUserInterfaceSystem;
enum class EWeaponAttackBaseType : uint8;
class UMontageHelper;
class ALocomotionSystem_CharacterBase;
struct FS_WeaponConfig;

UCLASS()
class HORRORZONE_API AWeaponBase : public AActor, public IInventoryItemInterface, public IWeaponInterface, public ILocomotionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	FORCEINLINE FS_WeaponConfig GetWeaponConfig() const { return WeaponConfig; }
	FORCEINLINE FS_MeleeWeaponConfigs GetMeleeWeaponConfigs() const { return MeleeWeaponConfigs; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// Interface Implementation
	// InventoryItemInterface
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
	
	// WeaponInterface
	virtual void Fire(bool bIsPressed) override;
	virtual void MeleeAttack(FHitResult HitResult) override;
	virtual void Melee(bool bIsPressed) override;
	virtual void Aim(bool bIsPressed) override;
	
	// LocomotionInterface
	virtual void Sprint(bool bIsPressed) override;
	
	EWeaponAttackBaseType GetWeaponAttackBaseType() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeHUD();

	void SetAnimSet();

	void AttachToCharacter();

	void SetWeaponMesh();

	void SetWeaponVisibility(bool bVisibile);

	void PlayWeaponEquipAnimMontage();

	void PlayWeaponUnequipAnimMontage();

	void PlayMontageOnOwner(UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection, bool ShouldStopAllMontages, FName ID);

	void MeleeAttackCombo(TArray<TObjectPtr<UAnimMontage>> AnimMontages );
	
	void PerformMeleeAttack(UAnimMontage* MontageToPlay);

	void MeleeSaveCombo();

	void MeleeResetCombo();

	void SetCrosshair();
	
	void UpdateCamera();
	
	bool CanAim();
	
	UFUNCTION()
	virtual void OnMontageCompletedAtOwner(FName AnimNotify);

	UFUNCTION()
	virtual void OnMontageBlendOutAtOwner(FName AnimNotify);

	UFUNCTION()
	virtual void OnMontageNotifyBeginAtOwner(FName AnimNotify, FName NotifyName);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	UUserInterfaceSystem* UserInterfaceSystem;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	bool bIsSprinting;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	bool bIsAiming;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	bool bSaveAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	int ComboIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	TArray<TObjectPtr<UAnimMontage>> ComboMeleeAttackAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	TObjectPtr<ULocomotionSystem> LocomotionSystem = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	ERotationMode CachedRotationMode;
};
