// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorProfileContainer.generated.h"

class UVerticalBox;
class USizeBox;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UActorProfileContainer : public UUserWidget
{
	GENERATED_BODY()
public:
	void AddElement(UUserWidget* Widget) const;
	
protected:
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* SizeBox;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* VBContainer;
};
