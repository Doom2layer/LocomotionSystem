// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LocomotionSystem_CharacterBase.generated.h"

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
	FORCEINLINE void SetLocomotionSystem(ULocomotionSystem* NewLocomotionSystem) { LocomotionSystem = NewLocomotionSystem; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ToggleWalk(bool IsPressed);

	virtual void ToggleCrouch(bool IsPressed);

	virtual void ToggleSprint(bool IsPressed);

	virtual void ToggleJump(bool IsPressed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	ULocomotionSystem* LocomotionSystem;
};
