// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Weapons/Firearm/FirearmBase.h"

#include "AIController.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Character/LocomotionSystem_CharacterBase.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/LocomotionSystem.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Widget/WeaponWidget.h"

void AFirearmBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeFirearmWeapon();
}

void AFirearmBase::Fire(bool bIsPressed)
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("FirearmBase::Fire - OwnerCharacter is null for %s"), *GetName());
		return;
	}

	bIsFiring = bIsPressed;
	if (bIsFiring && CanFire())
	{
		ULocomotionSystem* LocomotionSystem = OwnerCharacter->GetLocomotionSystem();
		if (!LocomotionSystem)
		{
			UE_LOG(LogTemp, Error, TEXT("FirearmBase::Fire - LocomotionSystem is null for %s"), *GetName());
			return;
		}

		CachedRotationMode = LocomotionSystem->GetRotationMode();
		LocomotionSystem->SetRotationMode(ERotationMode::Strafing);
		StartFire(FirearmWeaponConfig.bShouldDelay ? FirearmWeaponConfig.DelayTime : 0.0f);
	}
	else
	{
		StopFire();
		if (OwnerCharacter)
		{
			ULocomotionSystem* LocomotionSystem = OwnerCharacter->GetLocomotionSystem();
			if (LocomotionSystem)
			{
				LocomotionSystem->SetRotationMode(CachedRotationMode);
			}
		}
	}
}


void AFirearmBase::FireWeapon(TArray<FHitResult> HitResults)
{
	FireEffects(FilterHitResults(HitResults));
}

void AFirearmBase::FireEffects(const TArray<FVector>& ImpactPositions)
{
	
	if (ImpactPositions.Num() == 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Validate we have a tracer system
	if (!FirearmWeaponConfig.FireEffects.TracerSystem)
	{
		return;
	}

	// Get the weapon mesh for spawning projectiles
	UMeshComponent* WeaponMeshComp = GetWeaponMesh();
	if (!WeaponMeshComp)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (const FVector& ImpactPosition : ImpactPositions)
	{
		AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(
			AProjectileBase::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		if (Projectile)
		{
			Projectile->SetProjectileLifespan(FirearmWeaponConfig.FireEffects.ProjectileLifeSpan);
			Projectile->SetImpactPosition(ImpactPosition);
			Projectile->SetTracerSystem(FirearmWeaponConfig.FireEffects.TracerSystem);
			Projectile->SetWeaponMesh(WeaponMeshComp);
			Projectile->AttachToComponent(
				GetWeaponMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				NAME_None
			);
			Projectile->EmitProjectile();
		}
	}
}

UMeshComponent* AFirearmBase::GetWeaponMesh()
{
	// Check if StaticMesh is set and valid first (most weapons use static meshes)
	if (IsValid(WeaponStaticMesh))
	{
		return WeaponStaticMesh;
	}
    
	// Fall back to SkeletalMesh if StaticMesh isn't valid
	if (IsValid(WeaponSkeletalMesh))
	{
		return WeaponSkeletalMesh;
	}
	return nullptr;
}


TArray<FVector> AFirearmBase::FilterHitResults(TArray<FHitResult>& HitResults)
{
	TArray<FVector> L_ImpactPositions;
	
	for (const FHitResult& HitResult : HitResults)
	{
		L_ImpactPositions.Add(HitResult.ImpactPoint);
	}

	return L_ImpactPositions;
}

TArray<FHitResult> AFirearmBase::GetHitResult()
{
	TArray<FHitResult> HitResults;
	
	for (int i = 0; i < FirearmWeaponConfig.PelletCount; i++)
	{
		FHitResult HitResult;
		FVector StartLocation, EndLocation;
	
		GetTraceLocation(StartLocation, EndLocation);

		FCollisionQueryParams QueryParams;
	
		QueryParams.AddIgnoredActor(this);

		QueryParams.AddIgnoredActor(OwnerCharacter);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, QueryParams))
		{
			// Hit something - use the actual hit result
			HitResults.Add(HitResult);
		}
		else
		{
			// Didn't hit anything - use the end location as impact point
			HitResult.ImpactPoint = EndLocation;
			HitResult.ImpactNormal = -GetPlayerCameraTransform().GetRotation().GetForwardVector(); // Normal points back toward shooter
			HitResults.Add(HitResult);
		}
	}
	return HitResults;
}

void AFirearmBase::GetTraceLocation(FVector& StartLocation, FVector& EndLocation)
{
	StartLocation = GetPlayerCameraTransform().GetLocation();
	EndLocation = UKismetMathLibrary::RandomPointInBoundingBox(StartLocation, FirearmWeaponConfig.AccuracyLoss * FVector(1.0f, 1.0f, 1.0f)) + 
		(UUtilitiesFunctionLibrary::IsPlayer(OwnerCharacter) 
			? GetPlayerCameraTransform().GetRotation().GetForwardVector() * FirearmWeaponConfig.EffectiveRange
			: UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(GetPlayerCameraTransform().GetLocation(), GetAITargetLocation())) * FirearmWeaponConfig.EffectiveRange);
}

FTransform AFirearmBase::GetPlayerCameraTransform()
{

	if (UUtilitiesFunctionLibrary::IsPlayer(OwnerCharacter))
	{	
		UCameraComponent* CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();
		if (!CameraComp)
		{
			return FTransform::Identity;
		}

		return FTransform(
			CameraComp->GetComponentRotation().Quaternion(),
			CameraComp->GetComponentLocation(),
			FVector(1.0f)
		);
	}
	// for npcs, we can just use the actor's transform as a fallback (or you could implement a different logic for NPCs if needed)
	return OwnerCharacter->GetActorTransform();
}

void AFirearmBase::StartFire(float Delay)
{
	if (Delay <= 0.0f)
	{
		PerformShot();
		return;
	}
	 
	bShouldFire = true;
	AutoFireTick(Delay);
}

void AFirearmBase::StopFire()
{
	bShouldFire = false;
	GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
}

void AFirearmBase::AutoFireTick(float Delay)
{
	if (!bShouldFire || !CanFire())
	{
		// Exit retrigger loop if flag is false
		return;
	}
	 
	PerformShot();
	 
	// Retrigger delay timer
	GetWorldTimerManager().SetTimer(
		AutoFireTimerHandle,
		FTimerDelegate::CreateUObject(this, &AFirearmBase::AutoFireTick, Delay),
		Delay,
		false // single-shot timer, rescheduled manually
	);
}

void AFirearmBase::PerformShot()
{
	if (!IsMagazineEmpty())
	{
		PlayMontageOnOwner(FirearmWeaponConfig.FirearmWeaponAnims.Fire, 1.0f, 0.0f, NAME_None, true, "Weapon_Firearm_Fire");
		FireWeapon(GetHitResult());
		DecrementAmmo();
	}
	else
	{
		PlayMontageOnOwner(FirearmWeaponConfig.FirearmWeaponAnims.Fire, 1.0f, 0.0f, NAME_None, true, "Weapon_Firearm_DryFire");
		if (MaxDryFireTries > 0)
		{
			MaxDryFireTries--;
		}
		else
		{
			Reload();
			MaxDryFireTries = FirearmWeaponConfig.MaxDryFireTries;
		}
	}
}

void AFirearmBase::InitializeHUD()
{
	UUtilitiesFunctionLibrary::GetUserInterfaceSystem(GetWorld())->GetWeaponWidget()->Update(WeaponConfig.Icon, FirearmWeaponConfig.MagazineAmmo, true);
}

void AFirearmBase::InitializeFirearmWeapon()
{
	MagazineAmmo = FirearmWeaponConfig.MagazineAmmo;

	MagazineSize = FirearmWeaponConfig.MagazineSize;

	SpareAmmo = FirearmWeaponConfig.SpareAmmo;

	MaxDryFireTries = FirearmWeaponConfig.MaxDryFireTries;	
}

void AFirearmBase::DecrementAmmo()
{
	MagazineAmmo--;
	OnAmmoChangedDelegate.Broadcast(MagazineAmmo);
}

bool AFirearmBase::CanFire()
{
	return !IsReloading;
}

void AFirearmBase::Reload()
{
	if (!CanReload()) return;
	PlayMontageOnOwner(
		FirearmWeaponConfig.FirearmWeaponAnims.Reload,
		1.0f,
		0.0f,
		NAME_None,
		true,
		FName("Weapon_Firearm_Reload")
	);
}

void AFirearmBase::ReloadAmmo()
{
	
	IsReloading = true;
	bShouldFire = false;
	
	if (SpareAmmo > MagazineAmmo)
	{
		SpareAmmo = SpareAmmo - MagazineAmmo;
		MagazineAmmo = MagazineSize;
		OnAmmoChangedDelegate.Broadcast(MagazineAmmo);
	}
	else
	{
		MagazineAmmo = SpareAmmo;
		SpareAmmo = 0;
		OnAmmoChangedDelegate.Broadcast(MagazineAmmo);
	}
}

void AFirearmBase::OnMontageBlendOutAtOwner(FName AnimNotify)
{
	Super::OnMontageBlendOutAtOwner(AnimNotify);
	if (AnimNotify == FName("Weapon_Firearm_Reload"))
	{
		ReloadAmmo();
		IsReloading = false;
		bShouldFire = true;
	}
}

bool AFirearmBase::CanReload()
{
	if (MagazineAmmo >= MagazineSize)
	{
		return false;
	}
	return SpareAmmo > 0 ? true : false;
}

bool AFirearmBase::IsMagazineEmpty()
{
	return MagazineAmmo <= 0;
}

void AFirearmBase::Melee(bool bIsPressed)
{
	if (bIsPressed)
	{
		MeleeAttackCombo(MeleeWeaponConfigs.MeleeWeaponAnims.MeleeAttacks);
	}
}

FVector AFirearmBase::GetAITargetLocation()
{
	AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
	if (AIController)
	{
		if (UUtilitiesFunctionLibrary::GetCharacter(AIController->GetFocusActor()))
		{
			return UUtilitiesFunctionLibrary::GetCharacter(AIController->GetFocusActor())->GetMesh()->GetSocketLocation("spine_03");
		}
		return AIController->GetFocusActor()->GetActorLocation();
	}
	return FVector::ZeroVector;
}
