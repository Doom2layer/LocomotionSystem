// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LocomotionSystem_LayeredBlends.generated.h"

enum class EAnimBlendType : uint8;
/**
 * 
 */
UCLASS()
class HORRORZONE_API ULocomotionSystem_LayeredBlends : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	FORCEINLINE float GetBlendWeight() const { return BlendWeight; }
	FORCEINLINE void SetBlendWeight(float NewBlendWeight) { BlendWeight = NewBlendWeight; }
	FORCEINLINE float GetWeight() const { return Weight; }
	FORCEINLINE void SetWeight(float NewWeight) { Weight = NewWeight; }
	FORCEINLINE float GetInterpSpeed() const { return InterpSpeed; }
	FORCEINLINE void SetInterpSpeed(float NewInterpSpeed) { InterpSpeed = NewInterpSpeed; }
	FORCEINLINE float GetBlendTime() const { return BlendTime; }
	FORCEINLINE void SetBlendTime(float NewBlendTime) { BlendTime = NewBlendTime; }
	FORCEINLINE EAnimBlendType GetBlendType() const { return BlendType; }
	FORCEINLINE void SetBlendType(EAnimBlendType NewBlendType) { BlendType = NewBlendType; }
	
protected:
	
	UPROPERTY(BlueprintReadWrite)
	float BlendWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float InterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float BlendTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EAnimBlendType BlendType;
};
