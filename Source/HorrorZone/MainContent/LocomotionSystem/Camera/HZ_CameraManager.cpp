// Fill out your copyright notice in the Description page of Project Settings.

#include "MainContent/LocomotionSystem/Camera/HZ_CameraManager.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainContent/LocomotionSystem/Data/CameraSettings_Struct.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"

AHZ_CameraManager::AHZ_CameraManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CameraSettingsDT(TEXT("/Game/LocomotionSystem/Data/DataTables/Locomotion/DT_Camera.DT_Camera"));
	if (CameraSettingsDT.Succeeded())
	{
		CameraSettingsDataTable = CameraSettingsDT.Object;
	}

	// Create Timeline Component
	SpringArmLengthTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpringArmLengthTimeline"));
	FOVTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FOVTimeline"));
	TransformTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TransformTimeline"));
}

void AHZ_CameraManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (UUtilitiesFunctionLibrary::IsPlayer(PlayerCharacter.Get()))
	{
		SpringArmComponent = GetSpringArm();
		CameraComponent = GetFollowCamera();
        
		// Setup Timeline callbacks
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
        
		TimelineCallback.BindUFunction(this, FName("OnSpringArmLengthUpdate"));
		TimelineFinishedCallback.BindUFunction(this, FName("OnSpringArmLengthFinished"));
        
		SpringArmLengthTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
        
		InitializeCameraSettings();
	}
}

void AHZ_CameraManager::InitializeCameraSettings()
{
	if (!CameraSettingsDataTable)
	{
		return;
	}

	for (FName name : CameraSettingsDataTable->GetRowNames())
	{
		CachedCameraSettings.Add(name, *CameraSettingsDataTable->FindRow<FS_CameraSettings>(name, FString()));
	}
	UpdateCameraSettings(CameraState);
}

USpringArmComponent* AHZ_CameraManager::GetSpringArm()
{
	USpringArmComponent* L_SpringArmComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<USpringArmComponent>();
	if (IsValid(L_SpringArmComponent))
	{
		return L_SpringArmComponent;
	}
	return nullptr;	
}

UCameraComponent* AHZ_CameraManager::GetFollowCamera()
{
	UCameraComponent* L_FollowCameraComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<UCameraComponent>();
	if (IsValid(L_FollowCameraComponent))
	{
		return L_FollowCameraComponent;
	}
	return nullptr;
}

void AHZ_CameraManager::UpdateSpringArmLength(float TargetArmLength)
{
	if (SpringArmComponent)
	{
		SpringArmComponent->TargetArmLength = TargetArmLength;
	}
}

void AHZ_CameraManager::UpdateCameraTransform(FVector Location, FRotator Rotation)
{
	if (CameraComponent)
	{
		CameraComponent->SetRelativeLocationAndRotation(
			Location,
			Rotation
		);
	}
}

void AHZ_CameraManager::UpdateFOV(float TargetFOV)
{
	if (CameraComponent)
	{
		CameraComponent->SetFieldOfView(TargetFOV);
	}
}

void AHZ_CameraManager::UpdateCameraSettings(ECameraState CamState)
{
	FS_CameraSettings LCameraSettings = GetCameraSettings(CamState);
	
	// Update SpringArm Length - Use curve if available, otherwise update immediately
	if (LCameraSettings.TargetArmLengthCurve)
	{
		UpdateSpringArmLengthWithCurve(LCameraSettings.TargetArmLength, LCameraSettings.TargetArmLengthCurve);
	}
	else
	{
		UpdateSpringArmLength(LCameraSettings.TargetArmLength);
	}
    
	// Update Camera Transform - Use curve if available, otherwise update immediately
	if (LCameraSettings.TransformCurve)
	{
		UpdateCameraTransformWithCurve(
			LCameraSettings.CameraTransform.GetLocation(), 
			LCameraSettings.CameraTransform.GetRotation().Rotator(), 
			LCameraSettings.TransformCurve
		);
	}
	else
	{
		UpdateCameraTransform(LCameraSettings.CameraTransform.GetLocation(), LCameraSettings.CameraTransform.GetRotation().Rotator());
	}

	// Update FOV - Use curve if available, otherwise update immediately
	if (LCameraSettings.FOVCurve)
	{
		UpdateFOVWithCurve(LCameraSettings.CameraFOV, LCameraSettings.FOVCurve);
	}
	else
	{
		UpdateFOV(LCameraSettings.CameraFOV);
	}
}


FS_CameraSettings AHZ_CameraManager::GetCameraSettings(ECameraState CamState)
{
	for (const TPair<FName, FS_CameraSettings>& Pair : CachedCameraSettings)
	{
		if (Pair.Value.CameraState == CamState)
		{
			return Pair.Value;
		}
	}
	return FS_CameraSettings();
}

void AHZ_CameraManager::UpdateSpringArmLengthWithCurve(float NewTargetArmLength, UCurveFloat* Curve, float PlayRate)
{
	if (!SpringArmComponent || !Curve)
	{
		return;
	}
	
	// Store initial and target values
	InitialArmLength = SpringArmComponent->TargetArmLength;
	TargetArmLengthValue = NewTargetArmLength;

	// Clear existing curve and add new one
	SpringArmLengthTimeline->Stop();
	SpringArmLengthTimeline->SetPlayRate(PlayRate);
    
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("OnSpringArmLengthUpdate"));
	SpringArmLengthTimeline->AddInterpFloat(Curve, TimelineCallback);
    
	// Play timeline from start
	SpringArmLengthTimeline->PlayFromStart();
}

void AHZ_CameraManager::OnSpringArmLengthUpdate(float Value)
{
	if (SpringArmComponent)
	{
		// Lerp between initial and target arm length using curve value as alpha
		float NewArmLength = FMath::Lerp(InitialArmLength, TargetArmLengthValue, Value);
		SpringArmComponent->TargetArmLength = NewArmLength;
	}
}

void AHZ_CameraManager::OnSpringArmLengthFinished()
{
	// Optional: Do something when animation finishes
}

void AHZ_CameraManager::UpdateFOVWithCurve(float NewTargetFOV, UCurveFloat* Curve, float PlayRate)
{
	if (!CameraComponent || !Curve)
	{
		return;
	}
	
	// Store initial and target values
	InitialFOV = CameraComponent->FieldOfView;
	TargetFOVValue = NewTargetFOV;

	// Clear existing curve and add new one
	FOVTimeline->Stop();
	FOVTimeline->SetPlayRate(PlayRate);
    
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("OnFOVUpdate"));
	FOVTimeline->AddInterpFloat(Curve, TimelineCallback);
    
	// Play timeline from start
	FOVTimeline->PlayFromStart();
}

void AHZ_CameraManager::OnFOVUpdate(float Value)
{
	if (CameraComponent)
	{
		// Lerp between initial and target FOV using curve value as alpha
		float NewFOV = FMath::Lerp(InitialFOV, TargetFOVValue, Value);
		CameraComponent->SetFieldOfView(NewFOV);
	}
}

void AHZ_CameraManager::OnFOVFinished()
{
	// Optional: Do something when animation finishes
}

void AHZ_CameraManager::UpdateCameraTransformWithCurve(FVector NewLocation, FRotator NewRotation, UCurveFloat* Curve, float PlayRate)
{
	if (!CameraComponent || !Curve)
	{
		return;
	}
	
	// Store initial and target values
	InitialCameraLocation = CameraComponent->GetRelativeLocation();
	InitialCameraRotation = CameraComponent->GetRelativeRotation();
	TargetCameraLocation = NewLocation;
	TargetCameraRotation = NewRotation;

	// Clear existing curve and add new one
	TransformTimeline->Stop();
	TransformTimeline->SetPlayRate(PlayRate);
    
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("OnTransformUpdate"));
	TransformTimeline->AddInterpFloat(Curve, TimelineCallback);
    
	// Play timeline from start
	TransformTimeline->PlayFromStart();
}

void AHZ_CameraManager::OnTransformUpdate(float Value)
{
	if (CameraComponent)
	{
		// Lerp between initial and target location/rotation using curve value as alpha
		FVector NewLocation = FMath::Lerp(InitialCameraLocation, TargetCameraLocation, Value);
		FRotator NewRotation = FMath::Lerp(InitialCameraRotation, TargetCameraRotation, Value);
		CameraComponent->SetRelativeLocationAndRotation(NewLocation, NewRotation);
	}
}

void AHZ_CameraManager::OnTransformFinished()
{
	// Optional: Do something when animation finishes
}

