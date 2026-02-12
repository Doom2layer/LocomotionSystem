// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/WeaponWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Weapons/Firearm/FirearmBase.h"
#include "GameFramework/Character.h"

void UWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (UWeaponSystem* WeaponSystem = UUtilitiesFunctionLibrary::GetWeaponSystem(PlayerChar))
		{
			WeaponSystem->GetOnWeaponSlotChangedDelegate().AddDynamic(this, &UWeaponWidget::OnWeaponSlotChanged);
			
			if (AWeaponBase* CurrentWeapon = WeaponSystem->GetCurrentSlot())
			{
				OnWeaponSlotChanged(nullptr, CurrentWeapon);
			}
		}
	}
}

void UWeaponWidget::Update(UTexture2D* Icon, int Ammo, bool bShowAmmo)
{
	WeaponIcon->SetBrushFromTexture(Icon, true);
	AmmoText->SetText(FText::AsNumber(Ammo));
	AmmoText->SetVisibility(bShowAmmo ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UWeaponWidget::OnWeaponSlotChanged(AWeaponBase* PreviousWeapon, AWeaponBase* NewWeapon)
{
	if (PreviousWeapon)
	{
		if (AFirearmBase* PrevFirearm = Cast<AFirearmBase>(PreviousWeapon))
		{
			PrevFirearm->GetOnAmmoChangedDelegate().Clear();
		}
	}
 
	if (NewWeapon)
	{
		if (AFirearmBase* NewFirearm = Cast<AFirearmBase>(NewWeapon))
		{
			NewFirearm->GetOnAmmoChangedDelegate().Clear();
			NewFirearm->GetOnAmmoChangedDelegate().AddDynamic(this, &UWeaponWidget::OnAmmoChanged);
			Update(NewWeapon->GetWeaponConfig().Icon, NewFirearm->GetMagazineAmmo(), true);
		}
		else
		{
			Update(NewWeapon->GetWeaponConfig().Icon, 0, false);
		}
	}
}


void UWeaponWidget::OnAmmoChanged(int32 AmmoCount)
{
	AmmoText->SetText(FText::AsNumber(AmmoCount));
}

