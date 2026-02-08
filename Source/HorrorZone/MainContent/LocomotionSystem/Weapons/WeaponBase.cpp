// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"

#include "MainContent/LocomotionSystem/Character/LocomotionSystem_PlayerBase.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/LocomotionSystem.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
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

	OwnerCharacter = Cast<ALocomotionSystem_PlayerBase>(GetOwner());
}

void AWeaponBase::EquipItem()
{
	SetWeaponVisibility(true);
	InitializeHUD();
	SetAnimSet();
	AttachToCharacter();
}

void AWeaponBase::UnequipItem()
{
	SetWeaponVisibility(false);
}

void AWeaponBase::InitializeHUD()
{
	UUtilitiesFunctionLibrary::GetUserInterfaceSystem(GetWorld())->GetWeaponWidget()->Update(WeaponConfig.Icon, 0, false);
}

void AWeaponBase::SetAnimSet()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Setting animset to %s"), *WeaponConfig.AnimsetName.ToString()));
	UUtilitiesFunctionLibrary::GetLocomotionSystem(GetOwner())->SetAnimset(WeaponConfig.AnimsetName);	
}

void AWeaponBase::AttachToCharacter()
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponConfig.SocketName);
}

void AWeaponBase::SetWeaponMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("Setting weapon mesh for %s"), *GetName());
	WeaponStaticMesh->SetStaticMesh(WeaponConfig.WeaponStaticMesh);
	UE_LOG(LogTemp, Warning, TEXT("Set static mesh to %s"), *GetNameSafe(WeaponConfig.WeaponStaticMesh));
	WeaponSkeletalMesh->SetSkeletalMesh(WeaponConfig.WeaponSkeletalMesh);
	UE_LOG(LogTemp, Warning, TEXT("Set skeletal mesh to %s"), *GetNameSafe(WeaponConfig.WeaponSkeletalMesh));
}

void AWeaponBase::SetWeaponVisibility(bool bVisibile)
{
	SetActorHiddenInGame(!bVisibile);
}
