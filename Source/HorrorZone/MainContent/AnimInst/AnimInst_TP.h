// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInst_TP.generated.h"

class AHorrorZoneCharacter;
/**
 * 
 */
UCLASS()
class HORRORZONE_API UAnimInst_TP : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
	FVector SpotLightLocation;

	AHorrorZoneCharacter* Character;
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
