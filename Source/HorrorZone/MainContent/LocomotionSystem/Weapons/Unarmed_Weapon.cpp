// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Weapons/Unarmed_Weapon.h"

void AUnarmed_Weapon::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unarmed Weapon Begin Play"));
}
