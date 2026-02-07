// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Weapons/Pistol_Weapon.h"

void APistol_Weapon::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pistol Begin Play"));
}
