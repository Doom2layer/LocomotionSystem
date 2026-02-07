// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/LocomotionSystem_HUD.h"

#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"

ALocomotionSystem_HUD::ALocomotionSystem_HUD()
{
	UserInterfaceSystem = CreateDefaultSubobject<UUserInterfaceSystem>(TEXT("User Interface System"));
}
