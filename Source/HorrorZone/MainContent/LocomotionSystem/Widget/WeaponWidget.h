// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"

class UTextBlock;
class UImage;
class UOverlay;
class USizeBox;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Update(UTexture2D* Icon, int Ammo, bool bShowAmmo);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UOverlay* Overlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UImage* WeaponIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	UTextBlock* AmmoText;
};
