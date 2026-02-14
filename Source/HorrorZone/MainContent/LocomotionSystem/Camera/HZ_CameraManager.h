// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MainContent/LocomotionSystem/Data/CameraSettings_Struct.h"
#include "HZ_CameraManager.generated.h"

class UTimelineComponent;
enum class ECameraState : uint8;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class HORRORZONE_API AHZ_CameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AHZ_CameraManager();

	UFUNCTION(BlueprintCallable)
	void UpdateCameraSettings(ECameraState CameraState);

protected:
	virtual void BeginPlay() override;

	USpringArmComponent* GetSpringArm();

	UCameraComponent* GetFollowCamera();

	void UpdateSpringArmLength(float TargetArmLength);

	void UpdateCameraTransform(FVector Location, FRotator Rotation);

	void UpdateFOV(float TargetFOV = 90.0f);
	
	void InitializeCameraSettings();
    
	UFUNCTION()
	void OnSpringArmLengthUpdate(float Value);
    
	UFUNCTION()
	void OnSpringArmLengthFinished();
    
	void UpdateSpringArmLengthWithCurve(float NewTargetArmLength, UCurveFloat* Curve, float PlayRate = 1.0f);

	UFUNCTION()
	void OnFOVUpdate(float Value);
    
	UFUNCTION()
	void OnFOVFinished();
    
	void UpdateFOVWithCurve(float NewTargetFOV, UCurveFloat* Curve, float PlayRate = 1.0f);

	UFUNCTION()
	void OnTransformUpdate(float Value);
    
	UFUNCTION()
	void OnTransformFinished();
    
	void UpdateCameraTransformWithCurve(FVector NewLocation, FRotator NewRotation, UCurveFloat* Curve, float PlayRate = 1.0f);
	
	UPROPERTY()
    UTimelineComponent* SpringArmLengthTimeline;
    
    UPROPERTY()
    float InitialArmLength;
    
    UPROPERTY()
    float TargetArmLengthValue;

	UPROPERTY()
    UTimelineComponent* FOVTimeline;
    
    UPROPERTY()
    float InitialFOV;
    
    UPROPERTY()
    float TargetFOVValue;

	UPROPERTY()
    UTimelineComponent* TransformTimeline;
    
    UPROPERTY()
    FVector InitialCameraLocation;
    
    UPROPERTY()
    FRotator InitialCameraRotation;
    
    UPROPERTY()
    FVector TargetCameraLocation;
    
    UPROPERTY()
    FRotator TargetCameraRotation;
	
	FS_CameraSettings GetCameraSettings(ECameraState CamState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	TObjectPtr<UDataTable> CameraSettingsDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	TObjectPtr<ACharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	TMap<FName, FS_CameraSettings> CachedCameraSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	ECameraState CameraState;

};
