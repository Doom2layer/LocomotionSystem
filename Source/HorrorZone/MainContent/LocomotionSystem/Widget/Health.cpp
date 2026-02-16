// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/Health.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/ActorProfileSystem.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"

void UHealth::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UActorProfileSystem* ProfileSystem = UUtilitiesFunctionLibrary::GetActorProfileSystem(Owner))
	{
		float CurrentHealth = ProfileSystem->GetHealth();
		SetHealth(CurrentHealth, MaxHealth);
		
		ProfileSystem->GetOnHealthUpdatedDelegate().AddDynamic(this, &UHealth::OnHealthChanged);
	}
}

void UHealth::SetHealth(float InHealth, float InMaxHealth)
{
	MaxHealth = InMaxHealth;
	if (HealthBar)
	{
		HealthBar->SetPercent(UKismetMathLibrary::MapRangeClamped(InHealth, 0.0f, MaxHealth, 0.0f, 1.0f));
	}
}

void UHealth::OnHealthChanged(float Health)
{
	SetHealth(Health, MaxHealth);
}
