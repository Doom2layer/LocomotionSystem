// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LocomotionSystem_CharacterBase.generated.h"

class UActorProfileSystem;
class UWeaponSystem;
class UMontageHelper;
class ULocomotionSystem;

UCLASS()
class HORRORZONE_API ALocomotionSystem_CharacterBase : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ALocomotionSystem_CharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE ULocomotionSystem* GetLocomotionSystem() const { return LocomotionSystem; }
	FORCEINLINE UMontageHelper* GetMontageHelper() const { return MontageHelper; }
	FORCEINLINE UWeaponSystem* GetWeaponSystem() const { return WeaponSystem; }
	FORCEINLINE void SetLocomotionSystem(ULocomotionSystem* NewLocomotionSystem) { LocomotionSystem = NewLocomotionSystem; }
	FORCEINLINE void SetMontageHelper(UMontageHelper* NewMontageHelper) { MontageHelper = NewMontageHelper; }
	FORCEINLINE void SetWeaponSystem(UWeaponSystem* NewWeaponSystem) { WeaponSystem = NewWeaponSystem; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ToggleWalk(bool IsPressed);

	virtual void ToggleCrouch(bool IsPressed);

	virtual void ToggleSprint(bool IsPressed);

	virtual void ToggleJump(bool IsPressed);

	virtual void ToggleFire(bool IsPressed);

	virtual void ToggleMelee(bool IsPressed);

	virtual void ToggleAim(bool IsPressed);
	
	virtual void Reload();

	virtual void SwitchRotationMode(bool IsPressed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	ULocomotionSystem* LocomotionSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UMontageHelper* MontageHelper;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UWeaponSystem* WeaponSystem;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UActorProfileSystem* ActorProfileSystem;
};
