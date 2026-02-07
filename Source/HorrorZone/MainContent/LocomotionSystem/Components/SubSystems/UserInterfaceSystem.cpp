// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"
#include "Blueprint/UserWidget.h"
#include "MainContent/LocomotionSystem/Widget/WeaponWidget.h"

// Sets default values for this component's properties
UUserInterfaceSystem::UUserInterfaceSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UUserInterfaceSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Called when the game starts
void UUserInterfaceSystem::BeginPlay()
{
	Super::BeginPlay();

	CreateWeaponWidget();
	
}

void UUserInterfaceSystem::CreateWeaponWidget()
{
	if (WeaponWidget)
	{
		WeaponWidget->RemoveFromParent();
	}

	FString Path = "/Game/LocomotionSystem/Widget/WBP_Weapon.WBP_Weapon_C";

	auto WeaponClass = LoadClass<UUserWidget>(nullptr, *Path);

	WeaponWidget = CreateWidget<UWeaponWidget>(GetWorld(), WeaponClass);

	if (WeaponWidget)
	{
		WeaponWidget->AddToViewport();
	}
}
