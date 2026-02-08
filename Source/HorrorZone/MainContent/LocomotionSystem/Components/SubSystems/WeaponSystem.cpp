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
	WeaponSlots.Empty();
	
	if (!DefaultWeapons.IsEmpty())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;
		for (const TSubclassOf<AWeaponBase>& WeaponClass : DefaultWeapons)
		{
			AddWeapon(GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParameters));
		}
		UseWeapon(DefaultWeaponSlot);
	}
}

void UWeaponSystem::AddWeapon(TObjectPtr<AWeaponBase> Weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Attempting to add weapon %s"), *Weapon->GetName());
	if (WeaponSlots.Find(Weapon) == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Added weapon %s to slot %d"), *Weapon->GetName(), WeaponSlots.Num());
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
			UE_LOG(LogTemp, Warning, TEXT("Switched to weapon %s in slot %d"), *LWeapon->GetName(), Slot);
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
