// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UserInterfaceSystem.generated.h"


class UCrosshair;
class UWeaponWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORZONE_API UUserInterfaceSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUserInterfaceSystem();

	// Called Every Frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UWeaponWidget* GetWeaponWidget() const { return WeaponWidget; }
	FORCEINLINE UCrosshair* GetCrosshairWidget() const { return CrosshairWidget; }

	void ToggleCrosshair(bool bIsVisible);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CreateWeaponWidget();

	void CreateCrosshairWidget();

	bool ToggleWidgetVisibility(UUserWidget* Widget, bool bIsVisible);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget Reference", meta = (AllowPrivateAccess = "true"))
	UWeaponWidget* WeaponWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widget Reference", meta = (AllowPrivateAccess = "true"))
	UCrosshair* CrosshairWidget;
};
