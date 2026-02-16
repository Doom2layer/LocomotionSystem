// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/ActorProfileEffects/ActorProfileEffectsBase.h"
#include "HitReaction.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API AHitReaction : public AActorProfileEffectsBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser) override;
	
	void PlayAnimMontage(const ECardinalDirection Direction);
	
	UPROPERTY()	
	float HitDirectionAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HitReaction)
	TArray<TObjectPtr<UAnimMontage>> HitFWDReactionMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HitReaction)
	TArray<TObjectPtr<UAnimMontage>> HitBWDReactionMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HitReaction)
	TArray<TObjectPtr<UAnimMontage>> HitLeftReactionMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HitReaction)
	TArray<TObjectPtr<UAnimMontage>> HitRightReactionMontages;
};
