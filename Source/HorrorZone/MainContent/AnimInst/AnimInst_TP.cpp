// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/AnimInst/AnimInst_TP.h"

#include "HorrorZoneCharacter.h"

void UAnimInst_TP::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AHorrorZoneCharacter>(TryGetPawnOwner());
}

void UAnimInst_TP::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!Character) return;
	SpotLightLocation = Character->GetSpotLightLocation();
}
