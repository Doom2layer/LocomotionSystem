// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"WeaponConfig_Structs.generated.h"

enum class EWeapon : uint8;
/**
 * 
 */
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
};
