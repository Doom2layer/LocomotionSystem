// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/Helper/MontageHelper.h"

#include "PlayMontageCallbackProxy.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UMontageHelper::UMontageHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMontageHelper::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (IsValid(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
	{
		SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		Character = Cast<ACharacter>(Owner);
	}
}


// Called every frame
void UMontageHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMontageHelper::PlayMontage(UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection, bool ShouldStopAllMontages, FName ID)
{
	Event_PlayMontage(MontageToPlay, PlayRate, StartingPosition, StartingSection, ShouldStopAllMontages, ID);
}

void UMontageHelper::StopMontage()
{
	Event_StopMontage();
}


void UMontageHelper::Event_PlayMontage(UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection, bool ShouldStopAllMontages, FName ID)
{
	if (!SkeletalMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent is null. Cannot play montage."));
		return;
	}

	if (!MontageToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("MontageToPlay is null. Cannot play montage."));
		return;
	}
	CurrentMontageID = ID;
	UPlayMontageCallbackProxy* PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
	 SkeletalMeshComponent,
	 MontageToPlay,
	 PlayRate,
	 StartingPosition,
	 StartingSection,
	 ShouldStopAllMontages
	);
	PlayMontageCallbackProxy->OnCompleted.AddUniqueDynamic(this, &UMontageHelper::Completed);
	PlayMontageCallbackProxy->OnInterrupted.AddUniqueDynamic(this, &UMontageHelper::Intterupted);
	PlayMontageCallbackProxy->OnNotifyBegin.AddUniqueDynamic(this, &UMontageHelper::NotifyBegin);
	PlayMontageCallbackProxy->OnNotifyEnd.AddUniqueDynamic(this, &UMontageHelper::NotifyEnd);
	PlayMontageCallbackProxy->OnBlendOut.AddUniqueDynamic(this, &UMontageHelper::BlendOut);
}


void UMontageHelper::Intterupted(FName NotifyName)
{
	OnMontageInterrupted.Broadcast(CurrentMontageID);
}

void UMontageHelper::Completed(FName NotifyName)
{
	OnMontageCompleted.Broadcast(CurrentMontageID);
}

void UMontageHelper::NotifyBegin(FName NotifyName)
{
	OnMontageNotifyBegin.Broadcast(CurrentMontageID, NotifyName);
}

void UMontageHelper::NotifyEnd(FName NotifyName)
{
	OnMontageNotifyEnd.Broadcast(CurrentMontageID,NotifyName);
}

void UMontageHelper::BlendOut(FName NotifyName)
{
	OnMontageBlendOut.Broadcast(CurrentMontageID);
}

void UMontageHelper::Event_StopMontage()
{
	if (!Character) return;

	Character->StopAnimMontage();
}