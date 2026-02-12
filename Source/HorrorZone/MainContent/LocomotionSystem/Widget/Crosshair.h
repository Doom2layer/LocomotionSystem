// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

class USizeBox;
class UBorder;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCrosshair(FLinearColor InBorderColor, FLinearColor InCrosshairColor, float InRotation, float InLength, float InThickness, float InCrosshairSpread);

protected:

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;
	
	void SetBorderColors(FLinearColor InBorderColor, FLinearColor InCrosshairColor);

	void SetCrosshairRotation(float InRotation);

	void SetCrosshairSize(float InLength, float InThickness, float InCrosshairSpread);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float CrosshairSpread = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float CrosshairLength = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float CrosshairThickness = 4.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float CrosshairRotation = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FLinearColor BorderColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FLinearColor CrosshairColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	USizeBox* BaseSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UCanvasPanel* SizeboxCanvasPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* Top;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* TopChild;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* Bottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* BottomChild;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* LeftChild;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UBorder* RightChild;
};
