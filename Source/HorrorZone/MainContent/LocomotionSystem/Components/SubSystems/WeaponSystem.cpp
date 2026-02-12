// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"
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
	AActor* OwningActor = GetOwner();
	if (!OwningActor)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponSystem::Initialize - WeaponSystem has no owner"));
		return;
	}

	for (const auto& WeaponClass : DefaultWeapons)
	{
		if (WeaponClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OwningActor; // Set owner before spawn
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(
				WeaponClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				SpawnParams
			);

			if (Weapon)
			{
				// Double-check owner is set
				if (Weapon->GetOwner() == nullptr)
				{
					Weapon->SetOwner(OwningActor);
				}

				AddWeapon(Weapon);
			}
		}
	}
}


void UWeaponSystem::AddWeapon(TObjectPtr<AWeaponBase> Weapon)
{
	if (!Weapon) return;

	// Ensure weapon has correct owner
	if (Weapon->GetOwner() != GetOwner())
	{
		Weapon->SetOwner(GetOwner());
	}

	if (WeaponSlots.Find(Weapon) == INDEX_NONE)
	{
		int NewSlot = WeaponSlots.Add(Weapon);
		UseWeapon(NewSlot);
	}
}

void UWeaponSystem::UseWeapon(int Slot)
{
	if (!WeaponSlots.IsValidIndex(Slot))
	{
		return;
	}

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
	if (CurrentSlot >= 0 && CurrentSlot < WeaponSlots.Num() && WeaponSlots.IsValidIndex(CurrentSlot))
	{
		return WeaponSlots[CurrentSlot];
	}
	return nullptr;
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
	OnWeaponSlotChangedDelegate.Broadcast(PreviousWeaponClass, NewWeaponClass);
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

