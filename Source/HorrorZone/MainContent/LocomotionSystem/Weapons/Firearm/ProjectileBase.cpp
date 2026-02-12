// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Weapons/Firearm/ProjectileBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBase::EmitProjectile()
{
	if (TraceTrigger)
	{
		SpawnTracer();

		GetWorldTimerManager().SetTimer(LifeTimerHandle, [this]()
		{
			if (IsValid(SpawnedPSC))
			{
				Destroy();
			}
		}, ProjectileLifespan, false);
	}
}

void AProjectileBase::SpawnTracer()
{
	if (!WeaponMesh)
	{
		return;
	}

	if (!TracerSystem)
	{
		return;
	}

	// Check if socket exists
	if (!WeaponMesh->DoesSocketExist(TEXT("Muzzle")))
	{
		return;
	}

	FTransform SpawnTransform = WeaponMesh->GetSocketTransform(TEXT("Muzzle"), RTS_Actor);

	// Validate spawn transform
	if (SpawnTransform.GetLocation().IsNearlyZero())
	{
		return;
	}
	
	if (IsValid(SpawnedPSC) && SpawnedPSC->IsActive())
	{
		TraceTrigger = !TraceTrigger;
		SpawnedPSC->SetVectorParameter(TEXT("ImpactPosition"), ImpactPosition);
	}
	else
	{
		SpawnedPSC = UNiagaraFunctionLibrary::SpawnSystemAttached(
			 TracerSystem,
			 RootComponent,  // Attach to weapon mesh, not RootComponent
			 NAME_None,	 // Attach to the Muzzle socket
			 SpawnTransform.GetLocation(),  // Relative offset
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,  // Snap to socket location
			true,
			true,
			ENCPoolMethod::None,
			true
		);

		TraceTrigger = false;
		
		if (SpawnedPSC)
		{
			TraceTrigger = !TraceTrigger;
			SpawnedPSC->SetVectorParameter(TEXT("ImpactPosition"), ImpactPosition);
		}
	}
}
