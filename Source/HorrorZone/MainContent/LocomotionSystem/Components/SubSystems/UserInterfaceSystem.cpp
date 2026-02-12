// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"
#include "Blueprint/UserWidget.h"
#include "MainContent/LocomotionSystem/Widget/Crosshair.h"
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
	CreateCrosshairWidget();
	
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

void UUserInterfaceSystem::CreateCrosshairWidget()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->RemoveFromParent();
	}
	
	FString Path = "/Game/LocomotionSystem/Widget/WBP_Crosshair.WBP_Crosshair_C";

	auto CrosshairClass = LoadClass<UUserWidget>(nullptr, *Path);

	CrosshairWidget = CreateWidget<UCrosshair>(GetWorld(), CrosshairClass);

	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
}

void UUserInterfaceSystem::ToggleCrosshair(bool bIsVisible)
{
	if (ToggleWidgetVisibility(CrosshairWidget, bIsVisible))
	{
		
	}
	else
	{
		CreateCrosshairWidget();
	}
}


bool UUserInterfaceSystem::ToggleWidgetVisibility(UUserWidget* Widget, bool bIsVisible)
{
	if (IsValid(Widget))
	{
		Widget->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		return true; // Valid path
	}
	return false; // Invalid path
}
