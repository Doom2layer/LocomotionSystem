// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"

#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"

// Sets default values for this component's properties
UWeaponSystem::UWeaponSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UWeaponSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Called when the game starts
void UWeaponSystem::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	Initialize();
}

void UWeaponSystem::Initialize()
{
	WeaponSlots.Empty();

	if (!DefaultWeapons.IsEmpty())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
		for (const TSubclassOf<AWeaponBase>& WeaponClass : DefaultWeapons)
		{
			// Spawn deferred to allow Blueprint properties to be set
			AWeaponBase* NewWeapon = GetWorld()->SpawnActorDeferred<AWeaponBase>(
				WeaponClass,
				FTransform::Identity,
				Owner,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
            
			if (NewWeapon)
			{
				// This triggers OnConstruction and applies Blueprint properties
				NewWeapon->FinishSpawning(FTransform::Identity);
				AddWeapon(NewWeapon);
			}
		}
		UseWeapon(DefaultWeaponSlot);
	}
}

void UWeaponSystem::AddWeapon(TObjectPtr<AWeaponBase> Weapon)
{
	if (WeaponSlots.Find(Weapon) == INDEX_NONE)
	{
		UseWeapon(WeaponSlots.Add(Weapon));
	}
}

void UWeaponSystem::UseWeapon(int Slot)
{
	TObjectPtr<AWeaponBase> LWeapon = WeaponSlots[Slot];

	if (LWeapon)
	{
		TObjectPtr<AWeaponBase> LPreviousWeapon = GetCurrentSlot();
		
		if (LPreviousWeapon != LWeapon)
		{
			LWeapon->SetOwner(Owner);
			CurrentSlot = Slot;
			EquipUnequipWeapon(LPreviousWeapon, LWeapon);
		}
	}
}

TObjectPtr<AWeaponBase> UWeaponSystem::GetCurrentSlot()
{
	return CurrentSlot >= 0 ? WeaponSlots[CurrentSlot] : nullptr;
}

void UWeaponSystem::EquipUnequipWeapon(const TObjectPtr<AWeaponBase> PreviousWeaponClass, const TObjectPtr<AWeaponBase> NewWeaponClass)
{
	if (PreviousWeaponClass)
	{
		PreviousWeaponClass->UnequipItem();
	}
	if (NewWeaponClass)
	{
		NewWeaponClass->EquipItem();
	}
}

void UWeaponSystem::IncrementSlot()
{
	if (!WeaponSlots.IsEmpty())
	{
		UseWeapon(
			WeaponSlots.Num() - 1 == CurrentSlot ? 0 : CurrentSlot + 1
		);
	}
}

void UWeaponSystem::DecrementSlot()
{
	if (!WeaponSlots.IsEmpty())
	{
		UseWeapon(
			CurrentSlot == 0 ? WeaponSlots.Num() - 1 : CurrentSlot - 1
		);
	}
}

