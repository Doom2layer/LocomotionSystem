// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Data/Locomotion_Enums.h"
#include "MainContent/LocomotionSystem/Data/Locomotion_Structs.h"
#include "LocomotionSystem_AnimInst.generated.h"

enum class ERotationMode : uint8;
enum class EMovementType : uint8;
struct FAnimNodeReference;
struct FAnimUpdateContext;
enum class ERootYawOffsetMode : uint8;
class UCharacterMovementComponent;
enum class ECardinalDirection : uint8;
class ULocomotionSystem;
/**
 * 
 */
UCLASS()
class HORRORZONE_API ULocomotionSystem_AnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:
	/**
	 *  FUNCTIONS
	 */
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(Category = "Non Thread Safe Functions")
	void Initialization();

	UFUNCTION(Category = "Non Thread Safe Functions")
	void SetAnimationData();

	UFUNCTION(BlueprintCallable, Category = "Turn In Place", meta = (BlueprintThreadSafe))
	FORCEINLINE void SetTurnInPlaceB(bool NewTurnInPlaceB) { TurnInPlaceB = NewTurnInPlaceB; }

	UFUNCTION(BlueprintCallable, Category = "Turn In Place", meta = (BlueprintThreadSafe))
	FORCEINLINE void SetTurnInPlaceAnimTime(float NewTurnInPlaceAnimTime) { TurnInPlaceAnimTime = NewTurnInPlaceAnimTime; }
	
	UFUNCTION(BlueprintCallable, Category = "Turn In Place", meta = (BlueprintThreadSafe))
	FORCEINLINE void SetTurnInPlaceRotationDirection(float NewTurnInPlaceRotationDirection) { TurnInPlaceRotationDirection = NewTurnInPlaceRotationDirection; }

	UFUNCTION(BlueprintCallable, Category = "Turn In Place", meta = (BlueprintThreadSafe))
	FORCEINLINE void SetRootYawOffset(float NewRootYawOffset) { RootYawOffset = NewRootYawOffset; }

	UFUNCTION(BlueprintCallable, Category = "Turn In Place", meta = (BlueprintThreadSafe))
	FORCEINLINE void SetTurnYawCurveValue(float NewTurnYawCurveValue) { TurnYawCurveValue = NewTurnYawCurveValue; }

	UFUNCTION(BlueprintCallable, Category = "Turn In Place", meta = (BlueprintThreadSafe))
	FORCEINLINE void SetRootYawOffsetMode(ERootYawOffsetMode NewRootYawOffsetMode) { RootYawOffsetMode = NewRootYawOffsetMode; }
		
	/**
	 * VARIABLES
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference")
	APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference")
	ACharacter* Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	ULocomotionSystem* LocomotionSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion System")
	FS_Animset Animset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	ERotationMode RotationMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TArray<UAnimSequence*> IdleBreakAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	FS_Anim_Cardinal CycleCardinalAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	FS_Anim_Cardinal StartCardinalAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	FS_Anim_Cardinal StopCardinalAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	FS_Anim_ForwardFacingStarts ForwardFacingStartAnims;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	FS_Anim_Cardinal PivotCardinalAnims;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	FS_Anim_TurnInPlace TurnInPlace;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> SprintCycleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> JumpStartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> JumpStartLoopAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> JumpApexAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> JumpFallLoopAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> JumpFallLandAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> JumpRecoveryAdditiveAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> IdleToCrouchEntyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UAnimSequence> CrouchToIdleExitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UBlendSpace1D> Leans;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Sequences")
	TObjectPtr<UBlendSpace> AimOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	FVector2D YawRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	FVector2D PitchRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	FVector2D Offset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	float InterpSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	FName BlendName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	float BlendWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	float AimPitch;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming Data")
	float AimYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion System")
	FName LSAnimsetName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Idle Breaks")
	float TimeUntilSingleBreak;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Idle Breaks")
	float IdleBreakDelayTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Location Data")
	FVector WorldLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Location Data")
	float DisplacementSinceLastUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stride Warping")
	float StrideWarpingCycleAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stride Warping")
	float StrideWarpingStartAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stride Warping")
	float StrideWarpingPivotAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stride Warping")
	float TimeAtPivotStop;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance Matching")
	FVector2D PlayRateClampStarts;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance Matching")
	FVector2D PlayRateClampCycles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance Matching")
	FVector2D PlayRateClampPivots;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SlideWarpingBlendInOffset = 0.15f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SlideWarpingBlendInDurationScaled = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Location Data")
	float DisplacementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity Data")
	FVector WorldVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity Data")
	bool HasVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity Data")
	ECardinalDirection LastDirection = ECardinalDirection::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity Data")
	float LastVelocityDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Data")
	FRotator WorldRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Data")
	float YawDeltaSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Data")
	float AdditiveLeanAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Data")
	float YawDeltaSinceLastUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float CardinalDirectionDeadZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place ")
	bool DisableRootYawOffset;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place ")
	FVector2D RotationRootYawOffsetAngleClamp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place")
	bool DisableTurnInPlace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place")
	bool TurnInPlaceB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place")
	float TurnInPlaceAnimTime;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place")
	float TurnInPlaceRotationDirection;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place")
	float RootYawOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place")
	FFloatSpringState RootYawOffsetSpringState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turn In Place")
	float TurnYawCurveValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	float LocalVelocityDirectionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	float LocalVelocityDirectionWithOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	ECardinalDirection LocalVelocityDirectionNoOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	ECardinalDirection LocalVelocityDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pivots")
	ECardinalDirection PivotInitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pivots")
	FVector PivotStartingAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pivots")
	float LastPivotTime;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	FVector LocalVelocity2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	bool WasMovingLastUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Curves")
	FName Curve_TurnYawWeight = FName("TurnYawWeight");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Curves")
	FName Curve_RemainingTurnYaw = FName("RemainingTurnYaw");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Curves")
	FName Curve_LocomotionDistance = FName("Distance");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Acceleration Data")
	FVector LocalAcceleration2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Acceleration Data")
	bool HasAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Acceleration Data")
	FVector	PivotDirection2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Acceleration Data")
	ERootYawOffsetMode RootYawOffsetMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Acceleration Data")
	ECardinalDirection CardinalDirectionFromAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	EMovementType LS_MovementType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool IsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool MovementStateChanged;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool IsOnGround;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool IsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool IsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool CrouchStateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float TimeToJumpApex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float GroundTraceDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float TimeFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float LandRecoveryAlpha;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidJumpApexAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidJumpFallLandAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidJumpStartLoop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidAimOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStartFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStartBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStartLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStartRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStopFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStopBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStopLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidStopRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidPivotFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidPivotBwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidPivotLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Validities")
	bool IsValidPivotRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frame Checks")
	bool IsFirstUpdate = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool IsRunningIntoWall;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool InWall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool RotationModeChanged = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State Data")
	bool PlayStartAnim;

	
private:
	/**
	 * FUNCTIONS 
	 */
	UFUNCTION()
	void OnSetAnimSet(const FName& AnimsetName);

	UFUNCTION()
	void OnSetMovementType(EMovementType NewMovementType);

	UFUNCTION()
	void OnSetSprint(bool bSprint);

	UFUNCTION()
	void OnSetRotationMode(ERotationMode NewRotation);

	UFUNCTION()
	void OnAccelerationChanged(bool bHasAcceleration, float InAcceleration);
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Idle Breaks", meta = (BlueprintThreadSafe))
	bool CanPlayIdleBreaks() const;

	UFUNCTION(BlueprintCallable, Category = "Thread Safe Function", meta = (BlueprintThreadSafe))
	void UpdateLocationData(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Thread Safe Function", meta = (BlueprintThreadSafe))
	void UpdateVelocityData();
	
	UFUNCTION(BlueprintCallable, Category = "Thread Safe Function", meta = (BlueprintThreadSafe))
	void UpdateRotationData();
	
	UFUNCTION(BlueprintCallable, Category = "Idle Breaks", meta = (BlueprintThreadSafe))
	void ChooseBreakDelayTime();

	UFUNCTION(BlueprintCallable, Category = "Idle Breaks", meta = (BlueprintThreadSafe))
	void ResetIdleBreakTransitionLogic();

	UFUNCTION(BlueprintCallable, Category = "Idle Breaks", meta = (BlueprintThreadSafe))
	void ProcessIdleBreakTransitionLogic(float DeltaTime);

	UFUNCTION(Category= "Helper Functions", meta = (BlueprintThreadSafe))
	ECardinalDirection SelectCardinalDirectionFromAngle(float Angle, float DeadZone, ECardinalDirection CurrentDirection, bool UseCurrentDirection) const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Helper Functions", meta = (BlueprintThreadSafe))
	UAnimSequence* SelectTurnInPlaceAnimation(float Direction);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	bool IsForwardFacing() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	UCharacterMovementComponent* GetMovementComponent() const;

	UFUNCTION(Category= "Helper Functions", meta = (BlueprintThreadSafe))
	void UpdateAccelerationData();

	UFUNCTION(Category= "Helper Functions", meta = (BlueprintThreadSafe))
	void UpdateRootYawOffset(float DeltaTime);	

	UFUNCTION(Category= "Helper Functions", meta = (BlueprintThreadSafe))
	void OffsetRootYawOffset(float RootOffset);
	
	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	void ProcessTurnYawCurve();

	UFUNCTION(Category= "Helper Functions", meta = (BlueprintThreadSafe))
	FS_Anim_Movement GetMovementFromMovementType(EMovementType MovementType, FS_Anim_MovementGroup Movements);

	UFUNCTION(Category= "Helper Functions", meta = (BlueprintThreadSafe))
	void UpdateJumpFallDataLayer(float DeltaTime);
	
	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupIdleBreakAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateCycleAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupTurnInPlaceAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateTurnInPlaceAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateTurnInPlaceRecoveryAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupIdleTransitionAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupStartAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupStopAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	
	UFUNCTION(Category="StateNodeFunctions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupIdleStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="StateNodeFunctions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateIdleStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateStartAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateStopAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="StateNodeFunctions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupTurnInPlaceStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="StateNodeFunctions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="StateNodeFunctions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupLandRecoveryStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupPivotAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="AnimNodeFunction", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdatePivotAnimLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(Category="StateNodeFunctions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void SetupPivotStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	
	UFUNCTION(Category="StateNodeFunctions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdatePivotStateLayer(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	
	UFUNCTION(Category="Thread Safe Functions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateJumpFallData();
	
	UFUNCTION(Category="Thread Safe Functions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateCharacterStateData();

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	void SetAnimValidities();

	UFUNCTION(BlueprintCallable, Category= "Helper Functions")
	void SetGroundDistance();

	UFUNCTION(Category="Thread Safe Functions", BlueprintCallable, meta=(BlueprintThreadSafe))
	void UpdateAimingData(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	float GetAimOffsetPitch();

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	float GetAimOffsetYaw();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	bool CanStartMovingTransitionCheck() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	bool CanStopMovingTransitionCheck() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	bool CanPivotMovingTransitionCheck() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	bool IsMovingPerPendicularToInitialPivot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	bool IsVelocityOpposedToAcceleration() const;

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	void UpdateWallDetectionHeuristic();

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	bool ShouldDistanceMatchStop();

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	float GetPredictedStopDistance() const;

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	float GetPredictedPivotDistance() const;

	UFUNCTION(BlueprintCallable, Category= "Helper Functions", meta = (BlueprintThreadSafe))
	ECardinalDirection GetOppositeCardinalDirection(ECardinalDirection CurrentDirection) const;
	
};