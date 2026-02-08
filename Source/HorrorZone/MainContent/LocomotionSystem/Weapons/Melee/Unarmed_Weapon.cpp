// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Weapons/Melee/Unarmed_Weapon.h"

#include "MainContent/LocomotionSystem/Data/Weapons_Enums.h"

AUnarmed_Weapon::AUnarmed_Weapon()
{
	WeaponConfig.ID = EWeapon::Unarmed;
	WeaponConfig.Name = "Unarmed";
	WeaponConfig.Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/LSEssentials/Textures/Weapon/T_Unarmed.T_Unarmed"));
}


void AUnarmed_Weapon::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unarmed Weapon Begin Play"));
}
