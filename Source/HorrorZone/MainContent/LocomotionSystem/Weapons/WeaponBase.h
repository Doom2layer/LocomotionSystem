// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainContent/LocomotionSystem/Data/WeaponConfig_Structs.h"
#include "MainContent/LocomotionSystem/Interfaces/InventoryItemInterface.h"
#include "WeaponBase.generated.h"

class ALocomotionSystem_CharacterBase;
struct FS_WeaponConfig;

UCLASS()
class HORRORZONE_API AWeaponBase : public AActor, public IInventoryItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// Interface Implementation
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeHUD();

	void SetAnimSet();

	void AttachToCharacter();

	void SetWeaponMesh();

	void SetWeaponVisibility(bool bVisibile);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup)
	USceneComponent* RootSceneComponent;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup)
	FS_WeaponConfig WeaponConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	ALocomotionSystem_CharacterBase* OwnerCharacter;
};
