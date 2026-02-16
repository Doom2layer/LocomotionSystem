// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathEffect.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Components/Helper/MontageHelper.h"
#include "MainContent/LocomotionSystem/Data/Locomotion_Enums.h"

void ADeathEffect::OnDeath(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	PlayAnimMontage(GetCardinalDirectionFromAngel(GetHitDirectionAngle(ShotFromDirection)));
}

void ADeathEffect::PlayAnimMontage(const ECardinalDirection Direction)
{
	TObjectPtr<UAnimMontage> MontageToPlay = nullptr;
	
	switch (Direction)
	{
	case ECardinalDirection::Forward:
		MontageToPlay = DeathFWDReactionMontages.Num() > 0 ? DeathFWDReactionMontages[FMath::RandRange(0, DeathFWDReactionMontages.Num() - 1)] : nullptr;
		break;
	case ECardinalDirection::Backward:
		MontageToPlay = DeathBWDReactionMontages.Num() > 0 ? DeathBWDReactionMontages[FMath::RandRange(0, DeathBWDReactionMontages.Num() - 1)] : nullptr;
		break;
	case ECardinalDirection::Left:
		MontageToPlay = DeathLeftReactionMontages.Num() > 0 ? DeathLeftReactionMontages[FMath::RandRange(0, DeathLeftReactionMontages.Num() - 1)] : nullptr;
		break;
	case ECardinalDirection::Right:
		MontageToPlay = DeathRightReactionMontages.Num() > 0 ? DeathRightReactionMontages[FMath::RandRange(0, DeathRightReactionMontages.Num() - 1)] : nullptr;
		break;
	}
	// Play the montage on the owner's MontageHelper
	MontageHelper->PlayMontage(
		MontageToPlay,
		1.0f,
		0.0f,
		NAME_None,
		true,
		"DeathEffect"
	);
	
	Character->GetCharacterMovement()->DisableMovement();
}

void ADeathEffect::Ragdoll()
{
	Character->GetMesh()->SetCollisionProfileName("Ragdoll", true);
	Character->GetMesh()->SetAllBodiesBelowSimulatePhysics(RagdollImpulseBone, true, true);
 
	FVector LastUpdateVelocity = Character->GetCharacterMovement()->GetLastUpdateVelocity();
	FVector NormalizedVelocity = LastUpdateVelocity.GetSafeNormal(0.0001f);
	FVector ImpulseVector = LastUpdateVelocity + (NormalizedVelocity * RagdollImpulseStrength);
 
	Character->GetMesh()->AddImpulse(ImpulseVector, RagdollImpulseBone, true);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADeathEffect::OnMontageBlendOut(FName ID)
{
	if (ID == "DeathEffect")
	{
		Ragdoll();
	}
}


