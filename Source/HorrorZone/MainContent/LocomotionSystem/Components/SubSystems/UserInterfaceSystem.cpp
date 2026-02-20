// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/SubSystems/UserInterfaceSystem.h"

#include "ActorProfileSystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Widget/Crosshair.h"
#include "MainContent/LocomotionSystem/Widget/LSHUD.h"
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
	
	// Only initialize UI if there's a valid player pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UserInterfaceSystem: No player pawn found, skipping UI initialization"));
		return;
	}
	
	ToggleWeaponWidget(true);
	ToggleHUD(true);
	ToggleCrosshair(true);
}

void UUserInterfaceSystem::CreateWeaponWidget()
{
	if (WeaponWidget)
	{
		WeaponWidget->RemoveFromParent();
	}

	FString Path = "/Game/LocomotionSystem/Widget/WBP_Weapon.WBP_Weapon_C";

	auto Class = LoadClass<UUserWidget>(nullptr, *Path);

	WeaponWidget = CreateWidget<UWeaponWidget>(GetWorld(), Class);

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

	auto Class = LoadClass<UUserWidget>(nullptr, *Path);

	CrosshairWidget = CreateWidget<UCrosshair>(GetWorld(), Class);

	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
}

void UUserInterfaceSystem::CreateHUDWidget()
{
	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
	}
	
	FString Path = "/Game/LocomotionSystem/Widget/WBP_HUD.WBP_HUD_C";

	auto Class = LoadClass<UUserWidget>(nullptr, *Path);

	HUDWidget = CreateWidget<ULSHUD>(GetWorld(), Class);

	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
		
		// Safely get player pawn and ActorProfileSystem
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			HUDWidget->SetOwner(PlayerPawn);
			
			UActorProfileSystem* ProfileSystem = UUtilitiesFunctionLibrary::GetActorProfileSystem(PlayerPawn);
			if (ProfileSystem)
			{
				HUDWidget->OnHealthChanged(ProfileSystem->GetMaxHealth());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UserInterfaceSystem: ActorProfileSystem not found on player pawn"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UserInterfaceSystem: Player pawn is null, cannot initialize HUD"));
		}
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

void UUserInterfaceSystem::ToggleCrosshair(bool bIsVisible)
{
	if (!ToggleWidgetVisibility(CrosshairWidget, bIsVisible))
	{
		CreateCrosshairWidget();
	}
}

void UUserInterfaceSystem::ToggleHUD(bool bIsVisible)
{
	if (!ToggleWidgetVisibility(HUDWidget, bIsVisible))
	{
		CreateHUDWidget();
	}
}

void UUserInterfaceSystem::ToggleWeaponWidget(bool bIsVisible)
{
	if (!ToggleWidgetVisibility(WeaponWidget, bIsVisible))
	{
		CreateWeaponWidget();
	}
}
