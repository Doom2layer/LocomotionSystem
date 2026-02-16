// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"
#include "FirearmBase.generated.h"

class AProjectileBase;
enum class ERotationMode : uint8;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, AmmoCount);

UCLASS()
class HORRORZONE_API AFirearmBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	FORCEINLINE FOnAmmoChanged& GetOnAmmoChangedDelegate() {return OnAmmoChangedDelegate;}
	FORCEINLINE int32 GetMagazineAmmo() const { return MagazineAmmo; }
	
protected:
	virtual void BeginPlay() override;

	//Functions
	virtual void Fire(bool bIsPressed) override;

	virtual void InitializeHUD() override;

	virtual void Reload() override;

	virtual void OnMontageBlendOutAtOwner(FName AnimNotify) override;

	virtual void Melee(bool bIsPressed) override;

	void InitializeFirearmWeapon();
	
	void FireWeapon(TArray<FHitResult> HitResult);

	void StartFire(float Delay);
	
	void StopFire();

	void AutoFireTick(float Delay);

	void PerformShot();
	
	void GetTraceLocation(FVector& StartLocation, FVector& EndLocation);

	void DecrementAmmo();

	void FireEffects(const TArray<FVector>& ImpactPositions);

	void FireDamage(const TArray<FHitResult>& HitResults);
	
	void ReloadAmmo();
	
	bool CanFire();

	bool CanReload();

	bool IsMagazineEmpty();

	FVector GetAITargetLocation();

	UMeshComponent* GetWeaponMesh();

	TArray<FVector> FilterHitResults(TArray<FHitResult>& HitResults);
	
	TArray<FHitResult> GetHitResult();

	FTransform GetPlayerCameraTransform();
	
	// Delegates
	FOnAmmoChanged OnAmmoChangedDelegate;

	// Variables
	int32 MagazineAmmo = 0;

	int32 MagazineSize = 0;

	int32 SpareAmmo = 0;

	int32 MaxDryFireTries = 0;
	
	bool bShouldFire = false;

	bool IsReloading = false;
	
	FTimerHandle AutoFireTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup)
	FS_FirearmWeaponConfig FirearmWeaponConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Firing")
	bool bIsFiring;

};
