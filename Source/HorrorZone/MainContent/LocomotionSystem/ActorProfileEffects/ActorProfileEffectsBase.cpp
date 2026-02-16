// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/ActorProfileEffects/ActorProfileEffectsBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Animation/LocomotionSystem_AnimInst.h"
#include "MainContent/LocomotionSystem/Components/Helper/MontageHelper.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"

// Sets default values
AActorProfileEffectsBase::AActorProfileEffectsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorProfileEffectsBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the ActorProfileSystem from the owning actor
	if (GetOwner())
	{
		ActorProfileSystem = UUtilitiesFunctionLibrary::GetActorProfileSystem(GetOwner());
		if (ActorProfileSystem)
		{
			// Bind to the regular multicast delegate
			ActorProfileSystem->GetOnTakeDamageDelegate().AddDynamic(this, &AActorProfileEffectsBase::OnTakeDamage);
			ActorProfileSystem->GetOnDeathDelegate().AddDynamic(this, &AActorProfileEffectsBase::OnDeath);
		}
		Character = Cast<ACharacter>(GetOwner());
		ALocomotionSystem_CharacterBase* Player = Cast<ALocomotionSystem_CharacterBase>(GetOwner());
		MontageHelper = UUtilitiesFunctionLibrary::GetMontageHelper(Player);
		if (MontageHelper)
		{
			MontageHelper->GetOnMontageBlendOut().AddDynamic(this, &AActorProfileEffectsBase::OnMontageBlendOut);
		}
	}
}

// Called every frame
void AActorProfileEffectsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActorProfileEffectsBase::OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
}

float AActorProfileEffectsBase::GetHitDirectionAngle(FVector Location) const
{
	FVector LLocation = Location;
	if (Character)
	{
		return UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(Character->GetRootComponent()->GetComponentLocation(), LLocation), Character->GetRootComponent()->GetComponentRotation()).Yaw;
	}
	return 0.0f;
}

ECardinalDirection AActorProfileEffectsBase::GetCardinalDirectionFromAngel(float Angle)
{
	if (UKismetMathLibrary::InRange_FloatFloat(Angle, -45.0f, 45.0f))
	{
		return ECardinalDirection::Forward;
	}
	if (UKismetMathLibrary::InRange_FloatFloat(Angle, -135.0f, -45.0f))
	{
		return ECardinalDirection::Left;
	}
	if (UKismetMathLibrary::InRange_FloatFloat(Angle, 45.0f, 135.0f))
	{
		return ECardinalDirection::Right;
	}
	return ECardinalDirection::Backward;
}

void AActorProfileEffectsBase::OnDeath(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	
}

void AActorProfileEffectsBase::OnMontageBlendOut(FName ID)
{
	
}
