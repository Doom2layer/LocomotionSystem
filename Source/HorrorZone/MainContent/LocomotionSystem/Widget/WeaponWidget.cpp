// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/WeaponWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWeaponWidget::Update(UTexture2D* Icon, int Ammo, bool bShowAmmo)
{
	WeaponIcon->SetBrushFromTexture(Icon, true);
	AmmoText->SetText(FText::AsNumber(Ammo));
	if (!bShowAmmo) AmmoText->SetVisibility(ESlateVisibility::Hidden);
}
