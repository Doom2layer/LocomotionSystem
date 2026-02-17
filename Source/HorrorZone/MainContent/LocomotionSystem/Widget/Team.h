// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Team.generated.h"

class UTextBlock;
class USpacer;
class UBorder;
class UHorizontalBox;
class USizeBox;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UTeam : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTeamConfig(FLinearColor Color, FName TeamName);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> OuterSizeBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox;	

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> InnerSizeBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> TeamColor;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USpacer> Spacer;	
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TeamNameText;
};
