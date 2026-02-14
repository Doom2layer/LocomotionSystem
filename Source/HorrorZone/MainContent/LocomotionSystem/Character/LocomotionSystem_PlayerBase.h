// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainContent/LocomotionSystem/Character/LocomotionSystem_CharacterBase.h"
#include "LocomotionSystem_PlayerBase.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class HORRORZONE_API ALocomotionSystem_PlayerBase : public ALocomotionSystem_CharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	ALocomotionSystem_PlayerBase();

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoWalk();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJog();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoCrouch();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoUncrouch();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMontageCompleted(FName ID);

	UFUNCTION()
	void OnNotifyBegin(FName ID, FName NotifyName);

	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ReloadAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IncWeaponSlot;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* DecWeaponSlot;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FirearmMeleeAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SwitchRotationAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* TestMontage;	
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnOneKeyPressed();

	UFUNCTION(BlueprintCallable)
	void OnTwoKeyPressed();

	void IncWeapon();

	void DecWeapon();
	
	void Walk(const FInputActionValue& Value);

	void Jog(const FInputActionValue& Value);

	void MCrouch(const FInputActionValue& Value);
	
	void UCrouch(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);

	void USprint(const FInputActionValue& Value);

	void DJump(const FInputActionValue& Value);

	void UJump(const FInputActionValue& Value);

	void SwitchRotation(const FInputActionValue& Value);

	void DFire(const FInputActionValue& Value);

	void UFire(const FInputActionValue& Value);

	void DMelee(const FInputActionValue& Value);

	void UMelee(const FInputActionValue& Value);
	
	void DAim(const FInputActionValue& Value);
	
	void UAim(const FInputActionValue& Value);
};
