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

	void SetSprint(bool bSprint);

	void SetCharacterBaseAimRotation();

	void SetRotationMode(ERotationMode NewRotationMode);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetMovementConfigs();

	void SetMaxWalkSpeed(float Speed);

	void SetMaxAcceleration(float Acceleration);

	void SetRotationRate(const FRotator& NewRotationRate);

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
