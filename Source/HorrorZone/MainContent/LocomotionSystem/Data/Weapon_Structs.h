// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon_Structs.generated.h"

enum class EWeapon : uint8;
/**
 * 
 */
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
};

