// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MontageHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMontageCompleted, FName, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMontageInterrupted, FName, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMontageNotifyBegin, FName, ID, FName, NotifyName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMontageNotifyEnd, FName, ID, FName, NotifyName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMontageBlendOut, FName, ID);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORZONE_API UMontageHelper : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMontageHelper();

	FORCEINLINE FOnMontageCompleted& GetOnMontageCompleted() { return OnMontageCompleted; }
	FORCEINLINE FOnMontageInterrupted& GetOnMontageInterrupted() { return OnMontageInterrupted; }
	FORCEINLINE FOnMontageNotifyBegin& GetOnMontageNotifyBegin() { return OnMontageNotifyBegin; }
	FORCEINLINE FOnMontageNotifyEnd& GetOnMontageNotifyEnd() { return OnMontageNotifyEnd; }
	FORCEINLINE FOnMontageBlendOut& GetOnMontageBlendOut() { return OnMontageBlendOut; }
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void PlayMontage(UAnimMontage* MontageToPlay = nullptr, float PlayRate = 1.0f, float StartingPosition = 0.0f, FName StartingSection = NAME_None, bool ShouldStopAllMontages = true, FName ID = NAME_None);

	UFUNCTION()
	void StopMontage();
		
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void Event_PlayMontage(UAnimMontage* MontageToPlay = nullptr, float PlayRate = 1.0f, float StartingPosition = 0.0f, FName StartingSection = NAME_None, bool ShouldStopAllMontages = true, FName ID = NAME_None);

	UFUNCTION()
	void Event_StopMontage();

	UFUNCTION()
	void Intterupted(FName NotifyName);

	UFUNCTION()
	void Completed(FName NotifyName);
	
	UFUNCTION()
	void NotifyBegin(FName NotifyName);
	
	UFUNCTION()
	void NotifyEnd(FName NotifyName);
	
	UFUNCTION()
	void BlendOut(FName NotifyName);

	UPROPERTY(BlueprintAssignable)
	FOnMontageCompleted OnMontageCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnMontageInterrupted OnMontageInterrupted;

	UPROPERTY(BlueprintAssignable)
	FOnMontageNotifyBegin OnMontageNotifyBegin;

	UPROPERTY(BlueprintAssignable)
	FOnMontageNotifyEnd OnMontageNotifyEnd;

	UPROPERTY(BlueprintAssignable)
	FOnMontageBlendOut OnMontageBlendOut;
	
	UPROPERTY(EditAnywhere)
	AActor* Owner;

	UPROPERTY(EditAnywhere)
	ACharacter* Character;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY()
	FName CurrentMontageID;
};
