// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSHUD.generated.h"

class UProgressBar;
class UVerticalBox;
class USizeBox;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class HORRORZONE_API ULSHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetMaxHealth(const float InMaxHealth) { MaxHealth = InMaxHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE void SetOwner(AActor* InOwner) { Owner = InOwner; }
	FORCEINLINE AActor* GetOwner() const { return Owner; }

	UFUNCTION()
	void OnHealthChanged(float Health);
	
protected:
	virtual void NativeConstruct() override;
	
	void SetHealth(float InHealth, float InMaxHealth);
	
	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> Owner;
	
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> InnerSizeBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
};
