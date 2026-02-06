// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Animation/LocomotionSystem_LayeredBlends.h"

#include "Kismet/KismetMathLibrary.h"

void ULocomotionSystem_LayeredBlends::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	BlendWeight = UKismetMathLibrary::FInterpTo(BlendWeight,Weight, DeltaSeconds, InterpSpeed);
}
