// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/LSHUD.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/ActorProfileSystem.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"

void ULSHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULSHUD::SetHealth(float InHealth, float InMaxHealth)
{
	MaxHealth = InMaxHealth;
    
	// Bind delegate on first call when Owner is valid
	static bool bDelegateBound = false;
	if (!bDelegateBound && Owner)
	{
		UActorProfileSystem* ProfileSystem = UUtilitiesFunctionLibrary::GetActorProfileSystem(Owner);
		if (ProfileSystem)
		{
			ProfileSystem->GetOnHealthUpdatedDelegate().AddDynamic(this, &ULSHUD::OnHealthChanged);
			bDelegateBound = true;
		}
	}
    
	if (HealthBar)
	{
		HealthBar->SetPercent(UKismetMathLibrary::MapRangeClamped(InHealth, 0.0f, MaxHealth, 0.0f, 1.0f));
	}
}

void ULSHUD::OnHealthChanged(float Health)
{
	SetHealth(Health, MaxHealth);
}
