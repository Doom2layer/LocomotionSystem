// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"

#include "MainContent/LocomotionSystem/Camera/HZ_CameraManager.h"
#include "MainContent/LocomotionSystem/Character/LocomotionSystem_PlayerBase.h"
#include "MainContent/LocomotionSystem/Components/Helper/MontageHelper.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/LocomotionSystem.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"
#include "MainContent/LocomotionSystem/Data/Weapons_Enums.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Widget/Crosshair.h"
#include "MainContent/LocomotionSystem/Widget/WeaponWidget.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(RootSceneComponent);
	WeaponSkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponSkeletalMesh->CanCharacterStepUpOn = ECB_No;
	WeaponSkeletalMesh->SetGenerateOverlapEvents(false);

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	WeaponStaticMesh->SetupAttachment(RootSceneComponent);
	WeaponStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponStaticMesh->CanCharacterStepUpOn = ECB_No;
	WeaponStaticMesh->SetGenerateOverlapEvents(false);
}

void AWeaponBase::OnConstruction(const FTransform& Transform)
{
	SetWeaponMesh();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponBase: Owner is null in BeginPlay for %s"), *GetName());
		return;
	}

	// Cast to base character class instead of player-specific class
	OwnerCharacter = Cast<ALocomotionSystem_CharacterBase>(OwnerActor);

	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponBase: Owner is not ALocomotionSystem_CharacterBase for %s"), *GetName());
		return;
	}

	MontageHelper = UUtilitiesFunctionLibrary::GetMontageHelper(OwnerCharacter);
	
	if (!MontageHelper)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponBase: MontageHelper is null in BeginPlay for %s"), *GetName());
		return;
	}
    
	if (UUtilitiesFunctionLibrary::IsPlayer(GetOwner()))
	{
		UserInterfaceSystem = UUtilitiesFunctionLibrary::GetUserInterfaceSystem(GetWorld());
	}
    
	LocomotionSystem = UUtilitiesFunctionLibrary::GetLocomotionSystem(GetOwner());
	
	SetWeaponVisibility(false);
	MontageHelper->GetOnMontageCompleted().AddDynamic(this, &AWeaponBase::OnMontageCompletedAtOwner);
	MontageHelper->GetOnMontageBlendOut().AddDynamic(this, &AWeaponBase::OnMontageBlendOutAtOwner);
	MontageHelper->GetOnMontageNotifyBegin().AddDynamic(this, &AWeaponBase::OnMontageNotifyBeginAtOwner);
}


void AWeaponBase::EquipItem()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ALocomotionSystem_PlayerBase>(GetOwner());
        
		if (!OwnerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponBase::EquipItem - Cannot equip, no valid owner for %s"), *GetName());
			return;
		}
	}

	SetWeaponVisibility(true);
	if (UUtilitiesFunctionLibrary::IsPlayer(GetOwner()))
	{
		InitializeHUD();
		SetCrosshair();
		UpdateCamera();
	}
	SetAnimSet();
	AttachToCharacter();
	PlayWeaponEquipAnimMontage();
}

void AWeaponBase::UnequipItem()
{
	SetWeaponVisibility(false);
	PlayWeaponUnequipAnimMontage();
}

void AWeaponBase::InitializeHUD()
{
	UUtilitiesFunctionLibrary::GetUserInterfaceSystem(GetWorld())->GetWeaponWidget()->Update(WeaponConfig.Icon, 0, false);
}

void AWeaponBase::SetAnimSet()
{
	UUtilitiesFunctionLibrary::GetLocomotionSystem(GetOwner())->SetAnimset(WeaponConfig.AnimsetName);	
}

void AWeaponBase::AttachToCharacter()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ALocomotionSystem_PlayerBase>(GetOwner());
        
		if (!OwnerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponBase::AttachToCharacter - OwnerCharacter is null for %s"), *GetName());
			return;
		}
	}

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponConfig.SocketName);
}


void AWeaponBase::SetWeaponMesh()
{
	WeaponStaticMesh->SetStaticMesh(WeaponConfig.WeaponStaticMesh);
	WeaponSkeletalMesh->SetSkeletalMesh(WeaponConfig.WeaponSkeletalMesh);
}

void AWeaponBase::SetWeaponVisibility(bool bVisibile)
{
	SetActorHiddenInGame(!bVisibile);
}

void AWeaponBase::PlayWeaponEquipAnimMontage()
{
	if (!WeaponConfig.WeaponAnims.Equip)
	{
		UE_LOG(LogTemp, Warning, TEXT("Equip montage is null for weapon: %s, skipping montage play"), *GetName());
		return;
	}
	
	PlayMontageOnOwner(
		WeaponConfig.WeaponAnims.Equip,
		0.8f,
		0.0f,
		NAME_None,
		false,
		FName("Weapon_Equip")
	);
}

void AWeaponBase::PlayWeaponUnequipAnimMontage()
{
	if (!WeaponConfig.WeaponAnims.Unequip)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unequip montage is null for weapon: %s, skipping montage play"), *GetName());
		return;
	}
	
	PlayMontageOnOwner(
		WeaponConfig.WeaponAnims.Unequip,
		0.4f,
		0.0f,
		NAME_None,
		false,
		FName("Weapon_Unequip")
	);
}

void AWeaponBase::PlayMontageOnOwner(UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection, bool ShouldStopAllMontages, FName ID)
{
	if (MontageHelper)
	{
		MontageHelper->PlayMontage(MontageToPlay, PlayRate, StartingPosition, StartingSection, ShouldStopAllMontages, ID);
	}
}

void AWeaponBase::OnMontageCompletedAtOwner(FName AnimNotify)
{

}

void AWeaponBase::OnMontageBlendOutAtOwner(FName AnimNotify)
{

	if (AnimNotify == FName("Weapon_Unequip"))
	{
		SetWeaponVisibility(false);
	}
}

void AWeaponBase::OnMontageNotifyBeginAtOwner(FName AnimNotify, FName NotifyName)
{
	if (AnimNotify == "Weapon_MeleeAttack")
	{
		if (NotifyName == "MeleeResetCombo")
		{
			MeleeResetCombo();
		}
		else if (NotifyName == "MeleeSaveAttack")
		{
			MeleeSaveCombo();
		}
	}
}

void AWeaponBase::MeleeSaveCombo()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		ComboIndex = ComboIndex + 1 < ComboMeleeAttackAnimMontages.Num() ? ComboIndex + 1 : 0;
		PerformMeleeAttack(ComboMeleeAttackAnimMontages[ComboIndex]);
	}
}

void AWeaponBase::MeleeResetCombo()
{
	bIsAttacking = false;
	bSaveAttack = false;
	ComboIndex = 0;
}

void AWeaponBase::Fire(bool bIsPressed)
{
	if (bIsPressed)
	{
		MeleeAttackCombo(MeleeWeaponConfigs.MeleeWeaponAnims.MeleeAttacks);
	}
}

void AWeaponBase::MeleeAttack(FHitResult HitResult)
{
}

void AWeaponBase::Melee(bool bIsPressed)
{
}

void AWeaponBase::MeleeAttackCombo(TArray<TObjectPtr<UAnimMontage>> AnimMontages)
{
	ComboMeleeAttackAnimMontages = AnimMontages;

	if (!bIsAttacking)
	{
		bIsAttacking = true;
		PerformMeleeAttack(ComboMeleeAttackAnimMontages[ComboIndex]);
	}
	else
	{
		bSaveAttack = true;
	}
}

void AWeaponBase::PerformMeleeAttack(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay == nullptr) return;
	PlayMontageOnOwner(
		MontageToPlay,
		1.0f,
		0.0f,
		NAME_None,
		true,
		FName("Weapon_MeleeAttack")
	);
}

EWeaponAttackBaseType AWeaponBase::GetWeaponAttackBaseType() const
{
	 if (WeaponConfig.WeaponSkeletalMesh)
	{
		return EWeaponAttackBaseType::WeaponSkeletalMeshBased;
	}
	if (WeaponConfig.WeaponStaticMesh)
	{
		return EWeaponAttackBaseType::WeaponStaticMeshBased;
	}

	return EWeaponAttackBaseType::CharacterSkeletonBased;
}

void AWeaponBase::SetCrosshair()
{
	UserInterfaceSystem->ToggleCrosshair(WeaponConfig.CrosshairConfig.bIsEnabled);
	if (UserInterfaceSystem->GetCrosshairWidget()->IsVisible() && WeaponConfig.CrosshairConfig.bIsEnabled)
	{
		UserInterfaceSystem->GetCrosshairWidget()->SetCrosshair(
			WeaponConfig.CrosshairConfig.OutterBorderColor,
			WeaponConfig.CrosshairConfig.InnerBorderColor,
			WeaponConfig.CrosshairConfig.CrosshairRotation,
			WeaponConfig.CrosshairConfig.CrosshairLength,
			WeaponConfig.CrosshairConfig.CrosshairThickness,
			WeaponConfig.CrosshairConfig.CrosshairSpread
		);
	}
}

void AWeaponBase::UpdateCamera()
{
	Cast<AHZ_CameraManager>(UUtilitiesFunctionLibrary::GetCameraManager(GetWorld()))->UpdateCameraSettings(WeaponConfig.CameraConfig.DefaultCameraState);
}

void AWeaponBase::Sprint(bool bIsPressed)
{
	bIsSprinting = bIsPressed;
	if (LocomotionSystem)
	{
		LocomotionSystem->SetSprint(bIsSprinting);
		Cast<AHZ_CameraManager>(UUtilitiesFunctionLibrary::GetCameraManager(GetWorld()))->UpdateCameraSettings(bIsSprinting ? ECameraState::SprintCamera : WeaponConfig.CameraConfig.DefaultCameraState);
	}
}

void AWeaponBase::Aim(bool bIsPressed)
{
	bIsAiming = bIsPressed;
	if (LocomotionSystem)
	{
		if (CanAim())
		{
			Cast<AHZ_CameraManager>(UUtilitiesFunctionLibrary::GetCameraManager(GetWorld()))->UpdateCameraSettings(bIsAiming ? WeaponConfig.CameraConfig.CameraState : WeaponConfig.CameraConfig.DefaultCameraState);
			if (bIsAiming)
			{
				CachedRotationMode = LocomotionSystem->GetRotationMode();
				LocomotionSystem->SetRotationMode(ERotationMode::Strafing);
			}
			else
			{
				if (OwnerCharacter)
				{
					if (LocomotionSystem)
					{
						LocomotionSystem->SetRotationMode(CachedRotationMode);
					}
				}
			}
		}
	}
}

bool AWeaponBase::CanAim()
{
	return !bIsSprinting && WeaponConfig.CameraConfig.EnableAiming;
}
