// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/Crosshair.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"

void UCrosshair::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (IsDesignTime())
	{
		SetCrosshair(BorderColor, CrosshairColor, CrosshairRotation, CrosshairLength, CrosshairThickness, CrosshairSpread);
	}
}

void UCrosshair::NativeConstruct()
{
	SetCrosshair(BorderColor, CrosshairColor, CrosshairRotation, CrosshairLength, CrosshairThickness, CrosshairSpread);
}

void UCrosshair::SetCrosshair(FLinearColor InBorderColor, FLinearColor InCrosshairColor, float InRotation, float InLength, float InThickness, float InCrosshairSpread)
{
	SetBorderColors(InBorderColor, InCrosshairColor);
	SetCrosshairRotation(InRotation);
	SetCrosshairSize(InLength, InThickness, InCrosshairSpread);
}

void UCrosshair::SetBorderColors(FLinearColor InBorderColor, FLinearColor InCrosshairColor)
{
	Top->SetBrushColor(InBorderColor);
	Bottom->SetBrushColor(InBorderColor);
	Left->SetBrushColor(InBorderColor);
	Right->SetBrushColor(InBorderColor);
	TopChild->SetBrushColor(InCrosshairColor);
	BottomChild->SetBrushColor(InCrosshairColor);
	LeftChild->SetBrushColor(InCrosshairColor);
	RightChild->SetBrushColor(InCrosshairColor);
}

void UCrosshair::SetCrosshairRotation(float InRotation)
{
	BaseSizeBox->SetRenderTransformAngle(InRotation);
}

void UCrosshair::SetCrosshairSize(float InLength, float InThickness, float InCrosshairSpread)
{
	// Top
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Top)->SetSize(FVector2D(InThickness, InLength));
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Top)->SetPosition(FVector2D(0 - InThickness / 2.0f, 0.0 - InLength - InCrosshairSpread));
	
	// Bottom
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Bottom)->SetSize(FVector2D(InThickness, InLength));
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Bottom)->SetPosition(FVector2D(0 - InThickness / 2.0f, 0.0 + InCrosshairSpread));
	
	// Left
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Left)->SetSize(FVector2D(InLength, InThickness));
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Left)->SetPosition(FVector2D(0.0 - InLength - InCrosshairSpread, 0.0f - InThickness / 2.0f));
	
	// Right
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Right)->SetSize(FVector2D(InLength, InThickness));
	UWidgetLayoutLibrary::SlotAsCanvasSlot(Right)->SetPosition(FVector2D(0.0f + InCrosshairSpread, 0.0f - InThickness / 2.0f));
	
}
