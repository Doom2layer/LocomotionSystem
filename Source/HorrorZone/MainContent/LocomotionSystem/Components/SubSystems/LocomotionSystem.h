// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/Data/Locomotion_Structs.h"
#include "Components/ActorComponent.h"
#include "MainContent/LocomotionSystem/Data/Locomotion_Enums.h"
#include "LocomotionSystem.generated.h"

class UCharacterMovementComponent;
class ALocomotionSystem_CharacterBase;
enum class EMovementType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetAnimSet, const FName&, Animset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetMovementType, EMovementType, NewMovementType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetSprinting, bool, NewSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAccelerationChanged, bool, HasAcceleration, float, NewAcceleration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetRotationMode, ERotationMode, NewRotationMode);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HORRORZONE_API ULocomotionSystem : public UActorComponent
{
	GENERATED_BODY()

public:	

	FORCEINLINE UDataTable* GetDataTable() const { return Cast<UDataTable>(AnimsetDataTable); }
	FORCEINLINE FS_Animset* GetAnimset(const FName& RowName) const;
	FORCEINLINE FOnSetAnimSet& GetOnSetAnimSet() { return OnSetAnimSetDelegate; }
	FORCEINLINE FOnSetMovementType& GetOnSetMovementType() { return OnSetMovementTypeDelegate; }
	FORCEINLINE FOnSetSprinting& GetOnSetSprinting() { return OnSetSprintingDelegate; }
	FORCEINLINE FOnSetRotationMode& GetOnSetRotationMode() { return OnSetRotationModeDelegate; }
	FORCEINLINE FOnAccelerationChanged& GetOnAccelerationChanged() { return OnAccelerationChangedDelegate; }
	FORCEINLINE EMovementType GetMovementType() const { return MovementType; }
	FORCEINLINE FRotator GetBaseAimRotation() const { return BaseAimRotation; }
	FORCEINLINE bool GetHasAcceleration() const { return HasAcceleration; }
	FORCEINLINE bool GetIsSprinting() const { return IsSprinting; }
	FORCEINLINE ERotationMode GetRotationMode() const { return RotationMode; }
	FORCEINLINE void SetIsSprinting(bool NewSprinting) { IsSprinting = NewSprinting;}
	FORCEINLINE void SetDataTable(UDataTable* NewDataTable) { AnimsetDataTable = NewDataTable; }
	FORCEINLINE void SetAnimset(const FName& RowName);
    FORCEINLINE void SetMovementType(EMovementType NewMovementType);
	FORCEINLINE void SetOnSetSprintingDelegate(const FOnSetSprinting& Delegate) { OnSetSprintingDelegate = Delegate; }
	FORCEINLINE void SetBaseAimRotation(FRotator NewBaseAimRotation) { BaseAimRotation = NewBaseAimRotation; }
	FORCEINLINE void SetHasAcceleration(bool NewAcceleration) { HasAcceleration = NewAcceleration; }
	// Sets default values for this component's properties
	ULocomotionSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetSprint(bool bSprint);

	UFUNCTION()
	void SetCharacterBaseAimRotation();

	UFUNCTION()
	void SetRotationMode(ERotationMode NewRotationMode);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetMovementConfigs();

	UFUNCTION()
	void SetMaxWalkSpeed(float Speed);

	UFUNCTION()
	void SetMaxAcceleration(float Acceleration);

	UFUNCTION()
	void SetRotationRate(const FRotator& NewRotationRate);

	UFUNCTION()
	void CheckAcceleration();

	UFUNCTION()
	bool GetIsAccelerating();

	UFUNCTION()
	float GetLastVelocityDirection();

	UCharacterMovementComponent* GetMovementComponent() const;

	FS_Anim_Config GetMovementConfigsFromAnimSet();

	FS_Anim_Movement GetMovementFromMovementType(EMovementType InMovementType, FS_Anim_MovementGroup Movements);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Locomotion System | Setup")
	UDataTable* AnimsetDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Locomotion System | Setup")
	ERotationMode RotationMode;
	
	UPROPERTY()
	FName CurrentAnimsetRowName = FName("Unarmed");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion System | Setup")
	EMovementType MovementType = EMovementType::Jog;

	FOnSetAnimSet OnSetAnimSetDelegate;
	
	FOnSetMovementType OnSetMovementTypeDelegate;

	FOnSetSprinting OnSetSprintingDelegate;

	FOnSetRotationMode OnSetRotationModeDelegate;

	FOnAccelerationChanged OnAccelerationChangedDelegate;

	UPROPERTY(Transient)
	ACharacter* OwnerActor;

	UPROPERTY(Transient)
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY()
	bool IsSprinting;

	UPROPERTY()
	FRotator BaseAimRotation;

	UPROPERTY()
	bool HasAcceleration;
};
