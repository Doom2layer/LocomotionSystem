// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_WeaponAttackTrace.generated.h"

/**
 * 
 */
UCLASS()
class HORRORZONE_API UANS_WeaponAttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	void Trace(UMeshComponent* Mesh, FVector TraceStartLocation, FVector TraceEndLocation);

private:

	bool CanAttack(AActor* Actor);

	void GetTraceLocation(AActor* Owner, FVector& TraceStartLocation, FVector& TraceEndLocation);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	float AttackRadius = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	TMap<FString, bool> AttackableActors;
};
