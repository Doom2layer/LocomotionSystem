// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Animation/Animation Notifies/ANS_WeaponAttackTrace.h"

#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"
#include "MainContent/LocomotionSystem/Data/Weapons_Enums.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Weapons/WeaponBase.h"

void UANS_WeaponAttackTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
										float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FVector TraceStartLocation = FVector::ZeroVector;
	FVector TraceEndLocation = FVector::ZeroVector;
    
	GetTraceLocation(MeshComp->GetOwner(), TraceStartLocation, TraceEndLocation);
    
	if (!TraceStartLocation.IsZero() && !TraceEndLocation.IsZero())
	{
		Trace(MeshComp, TraceStartLocation, TraceEndLocation);
	}
}

void UANS_WeaponAttackTrace::Trace(UMeshComponent* Mesh, FVector TraceStartLocation, FVector TraceEndLocation)
{
	if (!Mesh)
	{
		return;
	}
	 
	TObjectPtr<UWorld> World = Mesh->GetWorld();
	
	if (World == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Mesh->GetOwner()); // Ignore self
	 
	bool bHit = World->SweepSingleByChannel(
		HitResult,
		TraceStartLocation,
		TraceEndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius), // Sphere radius for the trace
		Params
	);

	if (bHit)
	{
		if (CanAttack(HitResult.GetActor()))
		{
			UUtilitiesFunctionLibrary::GetWeaponSystem(Mesh->GetOwner())->GetCurrentSlot()->MeleeAttack(HitResult);
		}
	}
}

bool UANS_WeaponAttackTrace::CanAttack(AActor* Actor)
{
	if (Actor)
	{
		// if the fstring is in the map check if we can attack and return true or false, if the fstring isn't in the map return true 
		FString ActorClassName = Actor->GetClass()->GetName();
		if (AttackableActors.Contains(ActorClassName))
		{
			if (!AttackableActors[ActorClassName])
			{
				return false;
			}
			return true;
		}
		return true;
	}
	return false;
}

void UANS_WeaponAttackTrace::GetTraceLocation(AActor* Owner, FVector& TraceStartLocation, FVector& TraceEndLocation)
{
    if (!Owner)
    {
        return;
    }

    UWeaponSystem* WeaponSystem = UUtilitiesFunctionLibrary::GetWeaponSystem(Owner);
    if (!WeaponSystem)
    {
        return;
    }

    AWeaponBase* CurrentWeapon = WeaponSystem->GetCurrentSlot();
    if (!CurrentWeapon)
    {
        return;
    }

    USkeletalMeshComponent* OwnerMesh = Owner->GetComponentByClass<USkeletalMeshComponent>();
    if (!OwnerMesh)
    {
        return;
    }

    switch (CurrentWeapon->GetWeaponAttackBaseType())
    {
    case EWeaponAttackBaseType::CharacterSkeletonBased:
        TraceStartLocation = OwnerMesh->GetSocketLocation(CurrentWeapon->GetMeleeWeaponConfigs().MeleeTraceSocket.Start);
        TraceEndLocation = OwnerMesh->GetSocketLocation(CurrentWeapon->GetMeleeWeaponConfigs().MeleeTraceSocket.End);
        break;
    case EWeaponAttackBaseType::WeaponSkeletalMeshBased:
        {
            USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetComponentByClass<USkeletalMeshComponent>();
            if (WeaponMesh)
            {
                TraceStartLocation = WeaponMesh->GetSocketLocation(CurrentWeapon->GetMeleeWeaponConfigs().MeleeTraceSocket.Start);
                TraceEndLocation = WeaponMesh->GetSocketLocation(CurrentWeapon->GetMeleeWeaponConfigs().MeleeTraceSocket.End);
            }
        }
        break;
    case EWeaponAttackBaseType::WeaponStaticMeshBased:
        {
            UStaticMeshComponent* WeaponMesh = CurrentWeapon->GetComponentByClass<UStaticMeshComponent>();
            if (WeaponMesh)
            {
                TraceStartLocation = WeaponMesh->GetSocketLocation(CurrentWeapon->GetMeleeWeaponConfigs().MeleeTraceSocket.Start);
                TraceEndLocation = WeaponMesh->GetSocketLocation(CurrentWeapon->GetMeleeWeaponConfigs().MeleeTraceSocket.End);
            }
        }
        break;
    }
}
