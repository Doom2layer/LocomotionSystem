// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/ActorProfileEffects/HitReaction.h"

#include "MainContent/LocomotionSystem/Animation/LocomotionSystem_AnimInst.h"
#include "MainContent/LocomotionSystem/Components/Helper/MontageHelper.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/ActorProfileSystem.h"

void AHitReaction::OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                                UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
                                AActor* DamageCauser)
{
	Super::OnTakeDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection,
	                    DamageType,
	                    DamageCauser);

	if (!ActorProfileSystem->GetIsDead())
	{
		PlayAnimMontage(GetCardinalDirectionFromAngel(GetHitDirectionAngle(ShotFromDirection)));
	}	
}

void AHitReaction::PlayAnimMontage(const ECardinalDirection Direction)
{

	TObjectPtr<UAnimMontage> MontageToPlay = nullptr;
	
	switch (Direction)
	{
		case ECardinalDirection::Forward:
			MontageToPlay = HitFWDReactionMontages.Num() > 0 ? HitFWDReactionMontages[FMath::RandRange(0, HitFWDReactionMontages.Num() - 1)] : nullptr;
			break;
		case ECardinalDirection::Backward:
			MontageToPlay = HitBWDReactionMontages.Num() > 0 ? HitBWDReactionMontages[FMath::RandRange(0, HitBWDReactionMontages.Num() - 1)] : nullptr;
			break;
		case ECardinalDirection::Left:
			MontageToPlay = HitLeftReactionMontages.Num() > 0 ? HitLeftReactionMontages[FMath::RandRange(0, HitLeftReactionMontages.Num() - 1)] : nullptr;
			break;
		case ECardinalDirection::Right:
			MontageToPlay = HitRightReactionMontages.Num() > 0 ? HitRightReactionMontages[FMath::RandRange(0, HitRightReactionMontages.Num() - 1)] : nullptr;
			break;
	}

	// Play the montage on the owner's MontageHelper
	MontageHelper->PlayMontage(
		MontageToPlay,
		1.0f,
		0.0f,
		NAME_None,
		false,
		"HitReaction"
	);
}
