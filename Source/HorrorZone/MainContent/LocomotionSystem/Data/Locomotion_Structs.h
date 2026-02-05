// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Locomotion_Enums.h"
#include "Locomotion_Structs.generated.h"

class UBlendSpace1D;
/**
 * 
 */
USTRUCT(BlueprintType, meta=(DisplayName="S Anim Config"))
struct HORRORZONE_API FS_Anim_Config : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float MaxWalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float MaxAcceleration = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FVector2D RotationRootYawOffsetAngleClamp = FVector2D(-120.0f, 120.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	bool DisableTurnInPlace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Distance Matching")
	FVector2D PlayRateClampStarts = FVector2D(0.6f, 5.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Distance Matching")
	FVector2D PlayRateClampCycles = FVector2D(0.8f, 1.2f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Distance Matching")
	FVector2D PlayRateClampPivots = FVector2D(0.75f, 1.25f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation")
	FRotator RotationRate = FRotator(0.0f, 360.0f, 0.0f);
};


USTRUCT(BlueprintType, meta=(DisplayName="S Anim Transitions"))
struct HORRORZONE_API FS_Anim_Transitions : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Transitions")
	TObjectPtr<UAnimSequence> IdleToCrouchEntry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Transitions")
	TObjectPtr<UAnimSequence> CrouchToIdleExit;
	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Forward Facing Starts"))
struct HORRORZONE_API FS_Anim_ForwardFacingStarts : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> StartForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> StartForwardL90;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> StartForwardL180;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> StartForwardR90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> StartForwardR180;
};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Turn In Place"))
struct HORRORZONE_API FS_Anim_TurnInPlace : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> Left90 = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> Right90 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float PlayRate = 1.0f;
};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Cardinal"))
struct HORRORZONE_API FS_Anim_Cardinal : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> Forward = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> Backward = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> Left = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> Right = nullptr;
};

USTRUCT(BlueprintType)
struct HORRORZONE_API FS_AnimOverride_Sequence: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UAnimSequence> AnimationSequence = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float StartPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float OverrideWeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float InterSpeed = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float BlendTime = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	EAnimBlendType BlendType = EAnimBlendType::None;
};

USTRUCT(BlueprintType)
struct HORRORZONE_API FS_AnimOverride_ForwardFacingStarts: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FS_AnimOverride_Sequence StartForward = FS_AnimOverride_Sequence();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FS_AnimOverride_Sequence StartForwardL90 = FS_AnimOverride_Sequence();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FS_AnimOverride_Sequence StartForwardL180 = FS_AnimOverride_Sequence();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FS_AnimOverride_Sequence StartForwardR90 = FS_AnimOverride_Sequence();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FS_AnimOverride_Sequence StartForwardR180 = FS_AnimOverride_Sequence();
	
};

USTRUCT(BlueprintType)
struct HORRORZONE_API FS_AnimOverride_Movement : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FS_AnimOverride_ForwardFacingStarts ForwardFacingStarts = FS_AnimOverride_ForwardFacingStarts();
};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Movement"))
struct HORRORZONE_API FS_Anim_Movement : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Idle")
	TSoftObjectPtr<UAnimSequence> IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Idle Breaks")
	TArray<TSoftObjectPtr<UAnimSequence>> IdleBreaks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Start Cardinal")
	FS_Anim_Cardinal StartCardinal	= FS_Anim_Cardinal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forward Facing Starts")
	FS_Anim_ForwardFacingStarts ForwardFacingStarts = FS_Anim_ForwardFacingStarts();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cycle Cardinal")
	FS_Anim_Cardinal CycleCardinal = FS_Anim_Cardinal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stop Cardinal")
	FS_Anim_Cardinal StopCardinal = FS_Anim_Cardinal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Start Cardinal")
	FS_Anim_Cardinal PivotCardinal	= FS_Anim_Cardinal();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place")
	FS_Anim_TurnInPlace TurnInPlace = FS_Anim_TurnInPlace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configs")
	FS_Anim_Config Config = FS_Anim_Config();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Overrides")
	TMap<FName, FS_AnimOverride_Movement> AnimOverrides;

};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Movement Group"))
struct HORRORZONE_API FS_Anim_MovementGroup : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Walk")
	FS_Anim_Movement Walk;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jog")
	FS_Anim_Movement Jog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jog")
	FS_Anim_Movement Couch;

};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Sprint"))
struct HORRORZONE_API FS_Anim_Sprint : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint Cycle")
	UAnimSequence* SprintCycle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint Configs")
	FS_Anim_Config Configs = FS_Anim_Config();
};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Jump"))
struct HORRORZONE_API FS_Anim_Jump : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jump")
	TObjectPtr<UAnimSequence> JumpStart = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jump")
	TObjectPtr<UAnimSequence> JumpStartLoop = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jump")
	TObjectPtr<UAnimSequence> JumpApex = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jump")
	TObjectPtr<UAnimSequence> JumpFallLoop = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jump")
	TObjectPtr<UAnimSequence> JumpFallLand = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jump")
	TObjectPtr<UAnimSequence> JumpRecoveryAdditive = nullptr;
	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim Aim Offset"))
struct HORRORZONE_API FS_Anim_AimOffset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	TObjectPtr<UBlendSpace> AimOffsetBlendSpace = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	FVector2D YawRange = FVector2D(-180.0f, 180.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	FVector2D PitchRange = FVector2D(-90.0f, 90.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	FVector2D Offset = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	float InterpSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	FName BlendName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	float BlendWeight = 0.0f;
	
};

USTRUCT(BlueprintType, meta=(DisplayName="S Anim General"))
struct HORRORZONE_API FS_Anim_General : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint")
	FS_Anim_Sprint Sprint = FS_Anim_Sprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jump")
	FS_Anim_Jump Jump = FS_Anim_Jump();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transitions")
	FS_Anim_Transitions Transitions = FS_Anim_Transitions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Leaning")
	TObjectPtr<UBlendSpace1D> LeanBlendSpace = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	FS_Anim_AimOffset AimOffset = FS_Anim_AimOffset();
	
};

USTRUCT(BlueprintType, meta=(DisplayName="Animset"))
struct HORRORZONE_API FS_Animset : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	FS_Anim_General General = FS_Anim_General();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animset")
	FS_Anim_MovementGroup Movements;
};
