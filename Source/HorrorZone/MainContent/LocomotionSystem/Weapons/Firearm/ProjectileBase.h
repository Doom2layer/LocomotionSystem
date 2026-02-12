// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class HORRORZONE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	FORCEINLINE FVector GetImpactPosition() const { return ImpactPosition; }
	FORCEINLINE UNiagaraSystem* GetTracerSystem() const { return TracerSystem; }
	FORCEINLINE UMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetProjectileLifespan() const { return ProjectileLifespan; }
	FORCEINLINE void SetImpactPosition(const FVector& NewImpactPosition) { ImpactPosition = NewImpactPosition; }
	FORCEINLINE void SetTracerSystem(UNiagaraSystem* NewTracerSystem) { TracerSystem = NewTracerSystem; }
	FORCEINLINE void SetWeaponMesh(UMeshComponent* NewWeaponMesh) { WeaponMesh = NewWeaponMesh; }
	FORCEINLINE void SetProjectileLifespan(float NewProjectileLifespan) { ProjectileLifespan = NewProjectileLifespan; }
	
	void EmitProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnTracer();

	FTimerHandle LifeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	float ProjectileLifespan = 0.2f;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	FVector ImpactPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	TObjectPtr<UNiagaraComponent> SpawnedPSC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	TObjectPtr<UNiagaraSystem> TracerSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	TObjectPtr<UMeshComponent> WeaponMesh;

	bool TraceTrigger = true;
};
