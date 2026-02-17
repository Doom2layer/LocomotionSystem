// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/ActorProfileEffects/ActorProfileEffectsBase.h"
#include "DeathEffect.generated.h"

class UAnimMontage;
enum class ECardinalDirection : uint8;

/**
 * 
 */
UCLASS()
class HORRORZONE_API ADeathEffect : public AActorProfileEffectsBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnDeath(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser) override;
	
	void PlayAnimMontage(const ECardinalDirection Direction);
	
	void Ragdoll();

	virtual void OnMontageBlendOut(FName ID) override;
	
	FTimerHandle RagdollTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DeathEffect)
	float RagdollImpulseStrength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DeathEffect)
	FName RagdollImpulseBone = "pelvis";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DeathEffect)
	TArray<TObjectPtr<UAnimMontage>> DeathFWDReactionMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DeathEffect)
	TArray<TObjectPtr<UAnimMontage>> DeathBWDReactionMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DeathEffect)
	TArray<TObjectPtr<UAnimMontage>> DeathLeftReactionMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DeathEffect)
	TArray<TObjectPtr<UAnimMontage>> DeathRightReactionMontages;
};
