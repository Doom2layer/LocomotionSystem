// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraSettings_Struct.h"
#include "Weapon_Structs.generated.h"

enum class ECameraState : uint8;
enum class EWeapon : uint8;
class UNiagaraSystem;

/**
 * 
 */
USTRUCT(BlueprintType, meta=(DisplayName="S Weapon Damage Config"))
struct FS_WeaponDamageConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	float Damage = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();	
	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Weapon Camera Config"))
struct FS_WeaponCameraConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	bool EnableAiming = false;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	ECameraState DefaultCameraState = ECameraState::MediumCamera;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	ECameraState CameraState = ECameraState::AimingCloseCamera;
};

USTRUCT(BlueprintType, meta=(DisplayName="S Firearm Weapon Anims"))
struct FS_FirearmWeaponAnims : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	TObjectPtr<UAnimMontage> Fire = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	TObjectPtr<UAnimMontage> Reload = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	TObjectPtr<UAnimMontage> DryFire = nullptr;
};

USTRUCT(BlueprintType, meta=(DisplayName="S Firearm Weapon Fire Effects"))
struct FS_FirearmWeaponFireEffects : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	TObjectPtr<UNiagaraSystem> TracerSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	float ProjectileLifeSpan = 0.2f;
};

USTRUCT(BlueprintType, meta=(DisplayName="S Firearm Weapon Config"))
struct FS_FirearmWeaponConfig : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo")
	int32 MagazineAmmo = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo")
	int32 MagazineSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo")
	int32 SpareAmmo = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Delay")
	bool bShouldDelay = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Delay")
	float DelayTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Range")
	float EffectiveRange = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Accuracy")
	float AccuracyLoss = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pellets")
	int32 PelletCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pellets")
	int32 MaxDryFireTries = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile Effects")
	FS_FirearmWeaponFireEffects FireEffects = FS_FirearmWeaponFireEffects();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	FS_FirearmWeaponAnims FirearmWeaponAnims = FS_FirearmWeaponAnims();

};

USTRUCT(BlueprintType, meta=(DisplayName="S Weapon Crosshair Config"))
struct FS_WeaponCrosshairConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	bool bIsEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	FLinearColor OutterBorderColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	FLinearColor InnerBorderColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	float CrosshairRotation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	float CrosshairLength = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	float CrosshairThickness = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	float CrosshairSpread = 3.0f;
	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Weapon Anims"))
struct FS_WeaponAnims : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	TObjectPtr<UAnimMontage> Equip = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anims")
	TObjectPtr<UAnimMontage> Unequip = nullptr;
};

USTRUCT(BlueprintType, meta=(DisplayName="S Melee Trace Socket"))
struct FS_MeleeTraceSocket: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Socket")
	FName Start = "root";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Socket")
	FName End = "root";	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Melee Weapon Anims"))
struct FS_MeleeWeaponAnims : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	TArray<TObjectPtr<UAnimMontage>> MeleeAttacks;
	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Melee Weapon Configs"))
struct FS_MeleeWeaponConfigs: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FS_MeleeTraceSocket MeleeTraceSocket = FS_MeleeTraceSocket();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FS_MeleeWeaponAnims MeleeWeaponAnims = FS_MeleeWeaponAnims();
	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Weapon Config"))
struct HORRORZONE_API FS_WeaponConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	EWeapon ID;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FName Name = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	TObjectPtr<USkeletalMesh> WeaponSkeletalMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	TObjectPtr<UStaticMesh> WeaponStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FName AnimsetName = "Unarmed";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FName SocketName = "root";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FS_WeaponAnims WeaponAnims = FS_WeaponAnims();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FS_WeaponCrosshairConfig CrosshairConfig = FS_WeaponCrosshairConfig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FS_WeaponCameraConfig CameraConfig = FS_WeaponCameraConfig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FS_WeaponDamageConfig DamageConfig = FS_WeaponDamageConfig();
};

