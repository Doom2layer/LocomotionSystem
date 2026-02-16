// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorProfileEffectsBase.generated.h"

enum class ECardinalDirection : uint8;
class UMontageHelper;
class UActorProfileSystem;

UCLASS()
class HORRORZONE_API AActorProfileEffectsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorProfileEffectsBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	float GetHitDirectionAngle(FVector Location) const;
	
	ECardinalDirection GetCardinalDirectionFromAngel(float Angle);

	UFUNCTION()
	virtual void OnMontageBlendOut(FName ID);
	
	UFUNCTION()
	virtual void OnDeath(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION()
	virtual void OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
	UPROPERTY()
	TObjectPtr<UActorProfileSystem> ActorProfileSystem;
	
	UPROPERTY()
	TObjectPtr<UMontageHelper> MontageHelper;
	
	UPROPERTY()
	TObjectPtr<ACharacter> Character;
};
