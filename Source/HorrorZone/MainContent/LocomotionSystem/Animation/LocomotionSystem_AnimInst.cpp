// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Animation/LocomotionSystem_AnimInst.h"

#include "AnimationStateMachineLibrary.h"
#include "KismetAnimationLibrary.h"
#include "SequenceEvaluatorLibrary.h"
#include "SequencePlayerLibrary.h"
#include "Animation/AnimNodeReference.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimDistanceMatchingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AnimCharacterMovementLibrary.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/LocomotionSystem.h"
#include "MainContent/LocomotionSystem/Data/Locomotion_Enums.h"

void ULocomotionSystem_AnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Initialization();
}

void ULocomotionSystem_AnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsFalling)
	{
		SetGroundDistance();
	}
	if (IsRunningIntoWall)
	{
		//Delay until next tickU
		FTimerHandle L_TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(L_TimerHandle, [this]()
		{
			if (IsRunningIntoWall)
			{
				InWall = true;
			}
		}, 0.1f, false);
	}
	else
	{
		InWall = false;
	}
}
void ULocomotionSystem_AnimInst::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	UpdateLocationData(DeltaSeconds);
	UpdateRotationData();
	UpdateVelocityData();
	UpdateAccelerationData();
	UpdateWallDetectionHeuristic();
	UpdateRootYawOffset(DeltaSeconds);
	UpdateCharacterStateData();
	UpdateAimingData(DeltaSeconds);
	UpdateJumpFallDataLayer(DeltaSeconds);
	UpdateJumpFallData();
	IsFirstUpdate = false;
}

void ULocomotionSystem_AnimInst::Initialization()
{
	Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = Cast<ACharacter>(Pawn);
		LocomotionSystem = Cast<ULocomotionSystem>(Pawn->GetComponentByClass(ULocomotionSystem::StaticClass()));
        
		if (!LocomotionSystem) return;
        
		// Bind delegates BEFORE getting initial state
		LocomotionSystem->GetOnSetAnimSet().AddDynamic(this, &ULocomotionSystem_AnimInst::OnSetAnimSet);
		LocomotionSystem->GetOnSetMovementType().AddDynamic(this, &ULocomotionSystem_AnimInst::OnSetMovementType);
		LocomotionSystem->GetOnSetSprinting().AddDynamic(this, &ULocomotionSystem_AnimInst::OnSetSprint);
		// Set initial state - this should be false on initialization
		HasAcceleration = false;
	}
}

void ULocomotionSystem_AnimInst::OnSetAnimSet(const FName& AnimsetName)
{
	LSAnimsetName = AnimsetName;
	if (LocomotionSystem)
	{
		Animset = *LocomotionSystem->GetAnimset(LSAnimsetName);
	}
	SetAnimationData();
}

void ULocomotionSystem_AnimInst::OnSetMovementType(EMovementType MovementType)
{
	if (MovementType != LS_MovementType)
	{
		LS_MovementType = MovementType;
		SetAnimationData();
		MovementStateChanged = true;
		
		FTimerHandle L_TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(L_TimerHandle, [this]()
		{
			MovementStateChanged = false;
			if (GetMovementComponent()->IsCrouching() != IsCrouching)
			{
				IsCrouching = GetMovementComponent()->IsCrouching();
				CrouchStateChanged = true;
				FTimerHandle L_TimerHandle2;
				GetWorld()->GetTimerManager().SetTimer(L_TimerHandle2, [this]()
				{
					CrouchStateChanged = false;
				}, 0.1f, false);
			}
		}, 0.1f, false);
		
	}
}

void ULocomotionSystem_AnimInst::OnSetSprint(bool bSprint)
{
	IsSprinting = bSprint;
	SetAnimationData();
}

void ULocomotionSystem_AnimInst::SetAnimationData()
{
	if (!LocomotionSystem) return;

	FS_Anim_General L_GeneralStruct = Animset.General;
	
	FS_Anim_Movement L_MovementStruct = GetMovementFromMovementType(LS_MovementType, Animset.Movements);

	SprintCycleAnim = L_GeneralStruct.Sprint.SprintCycle;

	JumpStartAnim = L_GeneralStruct.Jump.JumpStart;

	JumpStartLoopAnim = L_GeneralStruct.Jump.JumpStartLoop;

	JumpApexAnim = L_GeneralStruct.Jump.JumpApex;

	JumpFallLoopAnim = L_GeneralStruct.Jump.JumpFallLoop;

	JumpFallLandAnim = L_GeneralStruct.Jump.JumpFallLand;

	JumpRecoveryAdditiveAnim = L_GeneralStruct.Jump.JumpRecoveryAdditive;

	IdleToCrouchEntyAnim = L_GeneralStruct.Transitions.IdleToCrouchEntry;

	CrouchToIdleExitAnim = L_GeneralStruct.Transitions.CrouchToIdleExit;

	Leans = L_GeneralStruct.LeanBlendSpace;

	AimOffset = L_GeneralStruct.AimOffset.AimOffsetBlendSpace;

	YawRange = L_GeneralStruct.AimOffset.YawRange;

	PitchRange = L_GeneralStruct.AimOffset.PitchRange;

	Offset = L_GeneralStruct.AimOffset.Offset;

	InterpSpeed = L_GeneralStruct.AimOffset.InterpSpeed;

	BlendName = L_GeneralStruct.AimOffset.BlendName;

	BlendWeight = L_GeneralStruct.AimOffset.BlendWeight;
	
	IdleAnimation = L_MovementStruct.IdleAnimation.LoadSynchronous();

	IdleBreakAnims.Empty();
	 
	for (const TSoftObjectPtr<UAnimSequence>& SoftSeq : L_MovementStruct.IdleBreaks)
	{
		UAnimSequence* SeqPtr = SoftSeq.LoadSynchronous(); // loads asset if needed
		if (SeqPtr)
		{
			IdleBreakAnims.Add(SeqPtr);
		}
	}

	CycleCardinalAnims = L_MovementStruct.CycleCardinal;

	StartCardinalAnims = L_MovementStruct.StartCardinal;

	StopCardinalAnims = L_MovementStruct.StopCardinal;

	PivotCardinalAnims = L_MovementStruct.PivotCardinal;	

	TurnInPlace = L_MovementStruct.TurnInPlace;

	RotationRootYawOffsetAngleClamp = L_MovementStruct.Config.RotationRootYawOffsetAngleClamp;

	PlayRateClampStarts = L_MovementStruct.Config.PlayRateClampStarts;

	PlayRateClampCycles = L_MovementStruct.Config.PlayRateClampCycles;

	PlayRateClampPivots = L_MovementStruct.Config.PlayRateClampPivots;
	
	DisableRootYawOffset = RotationRootYawOffsetAngleClamp.Equals(FVector2D(0.0f, 0.0f), 0.0f);
	
	DisableTurnInPlace = L_MovementStruct.Config.DisableTurnInPlace;

	SetAnimValidities();
}

bool ULocomotionSystem_AnimInst::CanPlayIdleBreaks() const
{
	return IdleBreakAnims.Num() > 0;
}

void ULocomotionSystem_AnimInst::UpdateLocationData(float DeltaTime)
{
	DisplacementSinceLastUpdate = (GetOwningActor()->GetActorLocation() - WorldLocation).Size2D();
	WorldLocation = GetOwningActor()->GetActorLocation();
	DisplacementSpeed = UKismetMathLibrary::SafeDivide(DisplacementSinceLastUpdate, DeltaTime);
	if (IsFirstUpdate)
	{
		DisplacementSinceLastUpdate = 0.0f;
		DisplacementSpeed = 0.0f;
	}
}

void ULocomotionSystem_AnimInst::ChooseBreakDelayTime()
{
	IdleBreakDelayTime = FMath::TruncToInt(FMath::Abs(WorldLocation.X + WorldLocation.Y)) % 10 + 5;
}

void ULocomotionSystem_AnimInst::ResetIdleBreakTransitionLogic()
{
	TimeUntilSingleBreak = IdleBreakDelayTime;
}

void ULocomotionSystem_AnimInst::ProcessIdleBreakTransitionLogic(float DeltaTime)
{
	if (CanPlayIdleBreaks())
	{
		TimeUntilSingleBreak -= DeltaTime;
	}
	else
	{
		ResetIdleBreakTransitionLogic();
	}
}

void ULocomotionSystem_AnimInst::UpdateVelocityData()
{
	WasMovingLastUpdate = !LocalVelocity2D.IsZero();
	
	if (!Pawn) return;
	WorldVelocity = Pawn->GetVelocity();

	FVector L_WorldVelocity2D = FVector(WorldVelocity.X, WorldVelocity.Y, 0.0f);

	LocalVelocity2D = WorldRotation.UnrotateVector(L_WorldVelocity2D);
	
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity, WorldRotation);

	LocalVelocityDirectionWithOffset = LocalVelocityDirectionAngle - RootYawOffset;

	LocalVelocityDirection = SelectCardinalDirectionFromAngle(LocalVelocityDirectionWithOffset, CardinalDirectionDeadZone, LocalVelocityDirection, WasMovingLastUpdate);
	
	LocalVelocityDirectionNoOffset = SelectCardinalDirectionFromAngle(LocalVelocityDirectionAngle, CardinalDirectionDeadZone, LocalVelocityDirectionNoOffset, WasMovingLastUpdate);

	HasVelocity = !FMath::IsNearlyEqual(LocalVelocity2D.SizeSquared(), 0.0f, 1e-9f);
	
}

void ULocomotionSystem_AnimInst::UpdateRotationData()
{
	if (!Pawn) return;

	YawDeltaSinceLastUpdate = GetOwningActor()->GetActorRotation().Yaw - WorldRotation.Yaw;
	YawDeltaSpeed = UKismetMathLibrary::SafeDivide(YawDeltaSinceLastUpdate, GetWorld()->GetDeltaSeconds());
	WorldRotation = GetOwningActor()->GetActorRotation();
	switch (LS_MovementType)
	{
	case EMovementType::None:
		AdditiveLeanAngle = YawDeltaSpeed * 1.0f;
		break;
	case EMovementType::Walk:
		AdditiveLeanAngle = YawDeltaSpeed * 0.025f;
		break;
	case EMovementType::Jog:
		AdditiveLeanAngle = YawDeltaSpeed * 0.035f;
		break;
	case EMovementType::Crouch:
		AdditiveLeanAngle = YawDeltaSpeed * 0.03f;
		break;
	}
	if (IsFirstUpdate)
	{
		AdditiveLeanAngle = 0.0f;
		YawDeltaSinceLastUpdate = 0.0f;
	}
}

ECardinalDirection ULocomotionSystem_AnimInst::SelectCardinalDirectionFromAngle(float Angle, float DeadZone, ECardinalDirection CurrentDirection, bool UseCurrentDirection) const
{
	float L_AbsAngle = FMath::Abs(Angle);
	float L_FwdDeadZone = DeadZone;
	float L_BwdDeadZone = DeadZone;

	if (UseCurrentDirection)
	{
		switch (CurrentDirection)
		{
		case ECardinalDirection::Forward:
			L_FwdDeadZone *= 2.0f;
			break;
		case ECardinalDirection::Backward:
			L_BwdDeadZone *= 2.0f;
			break;
		}
	}

	// Forward: angle is close to 0
	if (L_AbsAngle <= 45.0f + L_FwdDeadZone)
	{
		return ECardinalDirection::Forward;
	}
	// Backward: angle is close to 180 or -180
	if (L_AbsAngle >= 135.0f - L_BwdDeadZone)
	{
		return ECardinalDirection::Backward;
	}
	// Left/Right: determine by sign
	if (Angle > 0)
	{
		return ECardinalDirection::Right;
	}
	else
	{
		return ECardinalDirection::Left;
	}
}

UAnimSequence* ULocomotionSystem_AnimInst::SelectTurnInPlaceAnimation(float Direction)
{
	return Direction > 0 ? TurnInPlace.Right90 : TurnInPlace.Left90;
}

bool ULocomotionSystem_AnimInst::IsForwardFacing() const
{
	if (!Pawn) return false;
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	return MoveComp ? MoveComp->bOrientRotationToMovement : false;
}

UCharacterMovementComponent* ULocomotionSystem_AnimInst::GetMovementComponent() const
{
	if (!Pawn) return nullptr;
	return Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
}

void ULocomotionSystem_AnimInst::UpdateAccelerationData()
{
	if (!Pawn) return;
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!MoveComp)
	{
		LocalAcceleration2D = FVector::ZeroVector;
		HasAcceleration = false;
		return;
	}

	FVector LWorldAcceleration2D = MoveComp->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
	LocalAcceleration2D = WorldRotation.UnrotateVector(LWorldAcceleration2D);
	HasAcceleration = !FMath::IsNearlyEqual(LocalAcceleration2D.SizeSquared(), 0.0f, 1e-9f);
	PivotDirection2D = FMath::Lerp(PivotDirection2D, LocalAcceleration2D.GetSafeNormal(0.0001), 0.5f).GetSafeNormal();

	CardinalDirectionFromAcceleration = GetOppositeCardinalDirection(SelectCardinalDirectionFromAngle(UKismetAnimationLibrary::CalculateDirection(PivotDirection2D, WorldRotation), CardinalDirectionDeadZone, ECardinalDirection::Forward, false));
}

void ULocomotionSystem_AnimInst::UpdateRootYawOffset(float DeltaTime)
{
	if (RootYawOffsetMode == ERootYawOffsetMode::Accumulate)
	{
		RootYawOffset = RootYawOffset - YawDeltaSinceLastUpdate;
	}
	if (RootYawOffsetMode == ERootYawOffsetMode::BlendOut)
	{
		RootYawOffset = (UKismetMathLibrary::FloatSpringInterp(
			RootYawOffset,
			0.0f,
			RootYawOffsetSpringState,
			80.0f,
			1.0f,
			DeltaTime,
			1.0f,
			1.0f
		));
	}
	RootYawOffsetMode = ERootYawOffsetMode::BlendOut;
}

void ULocomotionSystem_AnimInst::OffsetRootYawOffset(float RootOffset)
{
	RootYawOffset = DisableTurnInPlace ? 0.0f :
	RotationRootYawOffsetAngleClamp.X == RotationRootYawOffsetAngleClamp.Y ?
	UKismetMathLibrary::NormalizeAxis(RootOffset) :
	UKismetMathLibrary::ClampAngle(UKismetMathLibrary::NormalizeAxis(RootOffset), RotationRootYawOffsetAngleClamp.X, RotationRootYawOffsetAngleClamp.Y);
}

void ULocomotionSystem_AnimInst::ProcessTurnYawCurve()
{
	float L_PreviousTurnYawCurveValue = TurnYawCurveValue;
		
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(GetCurveValue(Curve_TurnYawWeight), 0.0f, 0.0000000001f))
	{
		TurnYawCurveValue = 0.0f;
		L_PreviousTurnYawCurveValue = 0.0f;
	}
	else
	{
		TurnYawCurveValue = GetCurveValue(Curve_RemainingTurnYaw) / GetCurveValue(Curve_TurnYawWeight);
		if (L_PreviousTurnYawCurveValue != 0.0f)
		{
			RootYawOffset = RootYawOffset - (TurnYawCurveValue - L_PreviousTurnYawCurveValue);
		}
	}
}

void ULocomotionSystem_AnimInst::SetupIdleBreakAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		USequencePlayerLibrary::SetSequence(SequencePlayerReference, IdleBreakAnims[FMath::RandRange(0, IdleBreakAnims.Num() - 1)]);
	}
}

void ULocomotionSystem_AnimInst::UpdateCycleAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);

	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		UAnimSequence* SequenceToPlay = nullptr;
		switch (LocalVelocityDirectionNoOffset)
		{
		case ECardinalDirection::Forward:
			SequenceToPlay = IsSprinting ? SprintCycleAnim : CycleCardinalAnims.Forward;
			break;
		case ECardinalDirection::Backward:
			SequenceToPlay = CycleCardinalAnims.Backward;
			break;
		case ECardinalDirection::Left:
			SequenceToPlay = CycleCardinalAnims.Left;
			break;
		case ECardinalDirection::Right:
			SequenceToPlay = CycleCardinalAnims.Right;
			break;
		}
		UAnimDistanceMatchingLibrary::SetPlayrateToMatchSpeed(
		USequencePlayerLibrary::SetSequenceWithInertialBlending(Context, SequencePlayerReference, SequenceToPlay),
		DisplacementSpeed,
		PlayRateClampCycles
		);
		StrideWarpingCycleAlpha = UKismetMathLibrary::FInterpTo(StrideWarpingCycleAlpha, IsRunningIntoWall ? 0.5f : 1.0f, Context.GetContext()->GetDeltaTime(), 10.0f);
	}
}

void ULocomotionSystem_AnimInst::SetupTurnInPlaceAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	TurnInPlaceAnimTime = 0.0f;
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, 0.0f);
		TurnInPlaceB = !TurnInPlaceB;
	}
}

void ULocomotionSystem_AnimInst::UpdateTurnInPlaceAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		USequenceEvaluatorLibrary::SetSequenceWithInertialBlending(Context, SequenceEvaluatorReference, SelectTurnInPlaceAnimation(TurnInPlaceRotationDirection), 0.2f);
		TurnInPlaceAnimTime = (Context.GetContext()->GetDeltaTime() * TurnInPlace.PlayRate) + TurnInPlaceAnimTime;
		USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, TurnInPlaceAnimTime);
	}
}

void ULocomotionSystem_AnimInst::UpdateTurnInPlaceRecoveryAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		USequencePlayerLibrary::SetSequenceWithInertialBlending(Context, SequencePlayerReference, SelectTurnInPlaceAnimation(TurnInPlaceRotationDirection), 0.2f);
	}
}

void ULocomotionSystem_AnimInst::SetupIdleTransitionAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		USequencePlayerLibrary::SetSequence(SequencePlayerReference, IsCrouching ? IdleToCrouchEntyAnim : CrouchToIdleExitAnim);
	}
}

void ULocomotionSystem_AnimInst::SetupIdleStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	ChooseBreakDelayTime();
	ResetIdleBreakTransitionLogic();
}

void ULocomotionSystem_AnimInst::UpdateIdleStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult AnimationStateResult;
	FAnimationStateResultReference ResultReference;
	UAnimationStateMachineLibrary::ConvertToAnimationStateResult(Node, ResultReference, AnimationStateResult);
	if (AnimationStateResult == EAnimNodeReferenceConversionResult::Succeeded)
	{
		if(UAnimationStateMachineLibrary::IsStateBlendingOut(Context, ResultReference))
		{
			ResetIdleBreakTransitionLogic();
		}
		else
		{
			ProcessIdleBreakTransitionLogic(Context.GetContext()->GetDeltaTime());
		}
	}
}

void ULocomotionSystem_AnimInst::SetupTurnInPlaceStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	TurnInPlaceRotationDirection = UKismetMathLibrary::SignOfFloat(RootYawOffset) * -1.0f;
}

void ULocomotionSystem_AnimInst::UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult AnimationStateResult;
	FAnimationStateResultReference ResultReference;
	UAnimationStateMachineLibrary::ConvertToAnimationStateResult(Node, ResultReference, AnimationStateResult);
	if (AnimationStateResult == EAnimNodeReferenceConversionResult::Succeeded)
	{
		if(UAnimationStateMachineLibrary::IsStateBlendingOut(Context, ResultReference))
		{
			if (!IsForwardFacing())
			{
				TurnYawCurveValue = 0.0f;
			}
		}
		else
		{
			RootYawOffsetMode = ERootYawOffsetMode::Accumulate;
			ProcessTurnYawCurve();
		}
	}
}

void ULocomotionSystem_AnimInst::SetupStartAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		UAnimSequence* SequenceToPlay = nullptr;
		switch (LocalVelocityDirectionNoOffset)
		{
		case ECardinalDirection::Forward:
			SequenceToPlay = IsSprinting ? SprintCycleAnim : StartCardinalAnims.Forward;
			break;
		case ECardinalDirection::Backward:
			SequenceToPlay = StartCardinalAnims.Backward;
			break;
		case ECardinalDirection::Left:
			SequenceToPlay = StartCardinalAnims.Left;
			break;
		case ECardinalDirection::Right:
			SequenceToPlay = StartCardinalAnims.Right;
			break;
		}
  		USequenceEvaluatorLibrary::SetExplicitTime(USequenceEvaluatorLibrary::SetSequence(SequenceEvaluatorReference, SequenceToPlay), 0.0f);
		StrideWarpingStartAlpha = 0.0f;
	}
}

void ULocomotionSystem_AnimInst::UpdateStartAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		
		float L_ExplicitTime = USequenceEvaluatorLibrary::GetAccumulatedTime(SequenceEvaluatorReference);
		StrideWarpingStartAlpha = UKismetMathLibrary::MapRangeClamped(L_ExplicitTime - SlideWarpingBlendInOffset, 0.0f, SlideWarpingBlendInDurationScaled, 0.0f, 1.0f);
		UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(
			Context,
			SequenceEvaluatorReference,
			DisplacementSinceLastUpdate,
			Curve_LocomotionDistance,
			FVector2D(UKismetMathLibrary::Lerp(SlideWarpingBlendInDurationScaled, PlayRateClampStarts.X, StrideWarpingStartAlpha), PlayRateClampStarts.Y)
			);
	}
} 

void ULocomotionSystem_AnimInst::SetupStopAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		UAnimSequence* SequenceToPlay = nullptr;
		switch (LocalVelocityDirectionNoOffset)
		{
		case ECardinalDirection::Forward:
			SequenceToPlay = IsSprinting ? SprintCycleAnim : StopCardinalAnims.Forward;
			break;
		case ECardinalDirection::Backward:
			SequenceToPlay = StopCardinalAnims.Backward;
			break;
		case ECardinalDirection::Left:
			SequenceToPlay = StopCardinalAnims.Left;
			break;
		case ECardinalDirection::Right:
			SequenceToPlay = StopCardinalAnims.Right;
			break;
		}
  		USequenceEvaluatorLibrary::SetExplicitTime(USequenceEvaluatorLibrary::SetSequence(SequenceEvaluatorReference, SequenceToPlay), 0.0f);
		if (!ShouldDistanceMatchStop())
		{
			UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluatorReference, 0.0f, Curve_LocomotionDistance);
		}
	}
}

void ULocomotionSystem_AnimInst::UpdateStopAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		if (ShouldDistanceMatchStop())
		{
			float L_DistanceToMatch = GetPredictedStopDistance();
			if (L_DistanceToMatch > 0.0f)
			{
			UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluatorReference, L_DistanceToMatch, Curve_LocomotionDistance);
			}
		}
		else
		{
			USequenceEvaluatorLibrary::AdvanceTime(Context, SequenceEvaluatorReference, 1.0);
		}
	}
}

void ULocomotionSystem_AnimInst::SetupPivotAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	PivotStartingAcceleration = LocalAcceleration2D;
	
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		UAnimSequence* SequenceToPlay = nullptr;
		switch (LocalVelocityDirection)
		{
		case ECardinalDirection::Forward:
			SequenceToPlay = IsSprinting ? SprintCycleAnim : PivotCardinalAnims.Forward;
			break;
		case ECardinalDirection::Backward:
			SequenceToPlay = PivotCardinalAnims.Backward;
			break;
		case ECardinalDirection::Left:
			SequenceToPlay = PivotCardinalAnims.Left;
			break;
		case ECardinalDirection::Right:
			SequenceToPlay = PivotCardinalAnims.Right;
			break;
		}
		USequenceEvaluatorLibrary::SetExplicitTime(USequenceEvaluatorLibrary::SetSequence(SequenceEvaluatorReference, SequenceToPlay), 0.0f);
		StrideWarpingPivotAlpha = 0.0f;
		LastPivotTime = 0.0f;
		TimeAtPivotStop = 0.0f;
	}
}

void ULocomotionSystem_AnimInst::UpdatePivotAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	PivotStartingAcceleration = LocalAcceleration2D;
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (LastPivotTime > 0.0)
	{
		UAnimSequence* L_NewDesiredSequence = nullptr;
		switch (LocalVelocityDirection)
		{
		case ECardinalDirection::Forward:
			L_NewDesiredSequence = PivotCardinalAnims.Forward;
			break;
		case ECardinalDirection::Backward:
			L_NewDesiredSequence = PivotCardinalAnims.Backward;
			break;
		case ECardinalDirection::Left:
			L_NewDesiredSequence = PivotCardinalAnims.Left;
			break;
		case ECardinalDirection::Right:
			L_NewDesiredSequence = PivotCardinalAnims.Right;
			break;
		}

	if(USequenceEvaluatorLibrary::GetSequence(SequenceEvaluatorReference) != L_NewDesiredSequence)
	{
		USequenceEvaluatorLibrary::SetSequenceWithInertialBlending(Context, SequenceEvaluatorReference, L_NewDesiredSequence, 0.2f);
		PivotStartingAcceleration = LocalAcceleration2D;
	}
	}
	if (Result == EAnimNodeReferenceConversionResult::Succeeded)
	{
		float L_ExplicitTime = USequenceEvaluatorLibrary::GetAccumulatedTime(SequenceEvaluatorReference);
		if (IsVelocityOpposedToAcceleration())
		{
			UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluatorReference, GetPredictedPivotDistance(), Curve_LocomotionDistance);
			TimeAtPivotStop = L_ExplicitTime;
		}
		else
		{
			StrideWarpingPivotAlpha = UKismetMathLibrary::MapRangeClamped(L_ExplicitTime - TimeAtPivotStop - SlideWarpingBlendInOffset, 0.0f, SlideWarpingBlendInDurationScaled, 0.0f, 1.0f);
			UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(
				Context,
				SequenceEvaluatorReference,
				DisplacementSinceLastUpdate,
				Curve_LocomotionDistance,
				FVector2D(UKismetMathLibrary::Lerp(0.2f, PlayRateClampPivots.X, StrideWarpingPivotAlpha), PlayRateClampPivots.Y)
			);
		}
	}
}

void ULocomotionSystem_AnimInst::SetupPivotStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	PivotInitialDirection = LocalVelocityDirection;
}

void ULocomotionSystem_AnimInst::UpdatePivotStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	if (LastPivotTime > 0.0f)
	{
		LastPivotTime = LastPivotTime - Context.GetContext()->GetDeltaTime();
	}
}

FS_Anim_Movement ULocomotionSystem_AnimInst::GetMovementFromMovementType(EMovementType MovementType, FS_Anim_MovementGroup Movements)
{
	return  MovementType == EMovementType::Walk ? Movements.Walk :
		MovementType == EMovementType::Jog ? Movements.Jog :
		MovementType == EMovementType::Crouch ? Movements.Couch :
		FS_Anim_Movement();
}

void ULocomotionSystem_AnimInst::UpdateJumpFallData()
{
	// guard pawn and movement component
	if (!Pawn)
	{
		TimeToJumpApex = 0.0f;
		return;
	}

	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!MoveComp)
	{
		TimeToJumpApex = 0.0f;
		return;
	}

	if (IsJumping)
	{
		const float GravityZ = MoveComp->GetGravityZ();
		if (!FMath::IsNearlyZero(GravityZ))
		{
			TimeToJumpApex = (-WorldVelocity.Z) / GravityZ;
		}
		else
		{
			TimeToJumpApex = 0.0f;
		}
	}
	else
	{
		TimeToJumpApex = 0.0f;
	}
}

void ULocomotionSystem_AnimInst::UpdateCharacterStateData()
{
	// fetch movement component once and validate
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!MoveComp)
	{
		IsOnGround = false;
		IsJumping = false;
		IsFalling = false;
		IsCrouching = false;
		return;
	}

	IsOnGround = MoveComp->IsMovingOnGround();

	IsJumping = false;
	IsFalling = false;
	
	bool L_WasCrouchingLastUpdate = IsCrouching;

	IsCrouching = MoveComp->IsCrouching();

	CrouchStateChanged = IsCrouching != L_WasCrouchingLastUpdate;

	if (MoveComp->MovementMode == EMovementMode::MOVE_Falling)
	{
		if (WorldVelocity.Z > 0.0f)
		{
			IsJumping = true;
		}
		else
		{
			IsFalling = true;
		}
	}
	
}

void ULocomotionSystem_AnimInst::SetAnimValidities()
{
	IsValidJumpApexAnim = IsValid(JumpApexAnim);
	IsValidJumpFallLandAnim = IsValid(JumpFallLandAnim);
	IsValidJumpStartLoop = IsValid(JumpStartLoopAnim);
	IsValidAimOffset = IsValid(AimOffset);
	IsValidStartFwd = IsValid(StartCardinalAnims.Forward);
	IsValidStartBwd = IsValid(StartCardinalAnims.Backward);
	IsValidStartLeft = IsValid(StartCardinalAnims.Left);
	IsValidStartRight = IsValid(StartCardinalAnims.Right);
	IsValidStopFwd = IsValid(StopCardinalAnims.Forward);
	IsValidStopBwd = IsValid(StopCardinalAnims.Backward);
	IsValidStopLeft = IsValid(StopCardinalAnims.Left);
	IsValidStopRight = IsValid(StopCardinalAnims.Right);
	IsValidPivotFwd = IsValid(PivotCardinalAnims.Forward);
	IsValidPivotBwd = IsValid(PivotCardinalAnims.Backward);
	IsValidPivotLeft = IsValid(PivotCardinalAnims.Left);
	IsValidPivotRight = IsValid(PivotCardinalAnims.Right);
}

void ULocomotionSystem_AnimInst::SetGroundDistance()
{
	if (!Pawn) return;
	if (IsValid(Character))
	{
		float L_CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 20.0f;
	 
		FHitResult HitResult;
		FCollisionQueryParams Params(FName(TEXT("LineTrace")), true, GetOwningActor());
		Params.AddIgnoredActor(GetOwningActor());
	 
		FVector Start = Character->GetActorLocation() + (L_CapsuleHalfHeight * -1.0f) * Character->GetActorUpVector();
		FVector Target = Start + Character->GetActorUpVector() * (GroundTraceDistance * -1.0f);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, Target, ECC_Visibility, Params))
		{
			Distance = HitResult.Distance;
		}
	}
}

void ULocomotionSystem_AnimInst::UpdateJumpFallDataLayer(float DeltaTime)
{
	if (IsFalling)
	{
		TimeFalling += DeltaTime;
	}
	else
	{
		if (IsJumping)
		{
			TimeFalling = 0.0f;
		}
	}
}

void ULocomotionSystem_AnimInst::SetupLandRecoveryStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	LandRecoveryAlpha = IsCrouching ? UKismetMathLibrary::MapRangeClamped(TimeFalling, 0.0f, 0.4f, 0.1f, 10.f) * 5 : UKismetMathLibrary::MapRangeClamped(TimeFalling, 0.0f, 0.4f, 0.1f, 10.f);
}

void ULocomotionSystem_AnimInst::UpdateAimingData(float DeltaTime)
{
	if (!Pawn) return;

	float L_InterpSpeed = InterpSpeed;
	
	AimPitch = UKismetMathLibrary::FInterpTo(AimPitch, GetAimOffsetPitch(), DeltaTime, L_InterpSpeed);

	AimYaw = UKismetMathLibrary::FInterpTo(AimYaw, GetAimOffsetYaw(), DeltaTime, L_InterpSpeed);
}

float ULocomotionSystem_AnimInst::GetAimOffsetPitch()
{
	return FMath::Clamp(UKismetMathLibrary::NormalizeAxis(UKismetMathLibrary::NormalizedDeltaRotator(
	TryGetPawnOwner()->GetBaseAimRotation(),
	TryGetPawnOwner()->GetActorRotation()
	).Pitch),
	PitchRange.X,
	PitchRange.Y
	);
}

float ULocomotionSystem_AnimInst::GetAimOffsetYaw()
{
	return FMath::Clamp(DisableRootYawOffset ? UKismetMathLibrary::NormalizedDeltaRotator(
	TryGetPawnOwner()->IsPlayerControlled() ? TryGetPawnOwner()->GetBaseAimRotation() : LocomotionSystem->GetBaseAimRotation(),
	TryGetPawnOwner()->GetActorRotation()
	).Yaw : RootYawOffset * -1.0f,
	YawRange.X,
	YawRange.Y
	);
}

bool ULocomotionSystem_AnimInst::CanStartMovingTransitionCheck() const
{
	switch (LocalVelocityDirectionNoOffset)
	{
		case ECardinalDirection::Forward:
		return IsValidStartFwd;
		case ECardinalDirection::Backward:
		return IsValidStartBwd;
		case ECardinalDirection::Left:
		return IsValidStartLeft;
		case ECardinalDirection::Right:
		return IsValidStartRight;
	}
	return false;
}

bool ULocomotionSystem_AnimInst::CanStopMovingTransitionCheck() const
{
	switch (LocalVelocityDirectionNoOffset)
	{
		case ECardinalDirection::Forward:
		return IsValidStopFwd;
		case ECardinalDirection::Backward:
		return IsValidStopBwd;
		case ECardinalDirection::Left:
		return IsValidStopLeft;
		case ECardinalDirection::Right:
		return IsValidStopRight;
	}
	return false;
}

bool ULocomotionSystem_AnimInst::CanPivotMovingTransitionCheck() const
{
	switch (LocalVelocityDirection)
	{
	case ECardinalDirection::Forward:
		return IsValidPivotFwd;
	case ECardinalDirection::Backward:
		return IsValidPivotBwd;
	case ECardinalDirection::Left:
		return IsValidPivotLeft;
	case ECardinalDirection::Right:
		return IsValidPivotRight;
	}
	return false;
}

bool ULocomotionSystem_AnimInst::IsMovingPerPendicularToInitialPivot() const
{
	return ((PivotInitialDirection == ECardinalDirection::Forward || PivotInitialDirection == ECardinalDirection::Backward)
	&& !(LocalVelocityDirection == ECardinalDirection::Forward || LocalVelocityDirection == ECardinalDirection::Backward))
	|| ((PivotInitialDirection == ECardinalDirection::Left || PivotInitialDirection == ECardinalDirection::Right)
	&& !(LocalVelocityDirection == ECardinalDirection::Left || LocalVelocityDirection == ECardinalDirection::Right));
}

bool ULocomotionSystem_AnimInst::IsVelocityOpposedToAcceleration() const
{
	return UKismetMathLibrary::Dot_VectorVector(LocalVelocity2D, LocalAcceleration2D) < 0.0f;
}

void ULocomotionSystem_AnimInst::UpdateWallDetectionHeuristic()
{
	IsRunningIntoWall = LocalVelocity2D.Length() > 0.1 && LocalAcceleration2D.Length() > 0.1 && UKismetMathLibrary::InRange_FloatFloat(FVector::DotProduct(LocalVelocity2D.GetSafeNormal(), LocalAcceleration2D.GetSafeNormal()), -0.6f, 0.6f, true, true);
}

bool ULocomotionSystem_AnimInst::ShouldDistanceMatchStop()
{
	return HasVelocity && !HasAcceleration;
}

float ULocomotionSystem_AnimInst::GetPredictedStopDistance() const
{
	return UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
	GetMovementComponent()->GetLastUpdateVelocity(),
	GetMovementComponent()->bUseSeparateBrakingFriction,
	GetMovementComponent()->BrakingFriction,
	GetMovementComponent()->GroundFriction,
	GetMovementComponent()->BrakingFrictionFactor,
	GetMovementComponent()->BrakingDecelerationWalking
	).Size2D();
}

float ULocomotionSystem_AnimInst::GetPredictedPivotDistance() const
{
	return UAnimCharacterMovementLibrary::PredictGroundMovementPivotLocation(
		GetMovementComponent()->GetCurrentAcceleration(),
		GetMovementComponent()->GetLastUpdateVelocity(),
		GetMovementComponent()->GroundFriction
	).Size2D();
}

ECardinalDirection ULocomotionSystem_AnimInst::GetOppositeCardinalDirection(ECardinalDirection CurrentDirection) const
{
	//return the opposite of the input
	switch (CurrentDirection)
	{
	case ECardinalDirection::Forward:
		return ECardinalDirection::Backward;
	case ECardinalDirection::Backward:
		return ECardinalDirection::Forward;
	case ECardinalDirection::Left:
		return ECardinalDirection::Right;
	case ECardinalDirection::Right:
		return ECardinalDirection::Left;
	}
	return ECardinalDirection::Forward;
}