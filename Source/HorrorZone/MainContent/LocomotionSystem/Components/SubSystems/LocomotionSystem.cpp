// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/SubSystems/LocomotionSystem.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainContent/LocomotionSystem/Character/LocomotionSystem_CharacterBase.h"

// Sets default values for this component's properties
ULocomotionSystem::ULocomotionSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULocomotionSystem::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = Cast<ACharacter>(GetOwner());
	
	SetAnimset(CurrentAnimsetRowName);
	SetMovementType(MovementType);
}


// Called every frame
void ULocomotionSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	CheckAcceleration();
}

FS_Animset* ULocomotionSystem::GetAnimset(const FName& RowName) const
{
	// Load the DataTable asset synchronously if not yet loaded
	if (!AnimsetDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimsetDataTable is not loaded!"));
		return nullptr;
	}
	 
	// Attempt to find the row in the DataTable
	FS_Animset* AnimsetRow = AnimsetDataTable->FindRow<FS_Animset>(RowName, TEXT("GetAnimset"), true);
	if (!AnimsetRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Animset row '%s' not found!"), *RowName.ToString());
		return nullptr;
	}
	// Return pointer to the found row struct
	return AnimsetRow;
}

void ULocomotionSystem::SetAnimset(const FName& RowName)
{
	CurrentAnimsetRowName = RowName;
	OnSetAnimSetDelegate.Broadcast(CurrentAnimsetRowName);
}

void ULocomotionSystem::SetMovementType(EMovementType NewMovementType)
{
	MovementType = NewMovementType;
	SetMovementConfigs();
	OnSetMovementTypeDelegate.Broadcast(MovementType);
}

void ULocomotionSystem::SetMovementConfigs()
{
	// Add null check for MovementComponent
	if (!MovementComponent)
	{
		MovementComponent = GetMovementComponent();
		if (!MovementComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetMovementConfigs: MovementComponent is null"));
			return;
		}
	}

	FS_Anim_Config L_Config = GetMovementConfigsFromAnimSet();
    
	SetMaxWalkSpeed(L_Config.MaxWalkSpeed);
	SetMaxAcceleration(L_Config.MaxAcceleration);
	SetRotationRate(L_Config.RotationRate);
}

FS_Anim_Config ULocomotionSystem::GetMovementConfigsFromAnimSet()
{
	FS_Animset* Animset = GetAnimset(CurrentAnimsetRowName);
	if (!Animset)
	{
		UE_LOG(LogTemp, Error, TEXT("GetMovementConfigsFromAnimSet: Animset is null for row '%s'"), *CurrentAnimsetRowName.ToString());
		return FS_Anim_Config(); 
	}
	if (IsSprinting)
	{
		return Animset->General.Sprint.Configs;
	}
	if (GetCurrentOverrideStruct().Configs.Enabled)
	{
		//return overriden configs
		return GetCurrentOverrideStruct().Configs.Configs;
	}
	//return global configs
	return GetCurrentMovementStruct().Config;
}

void ULocomotionSystem::SetAnimOverride(const FName& NewAnimOverride)
{
	AnimOverride = NewAnimOverride;
	OnSetAnimOverrideDelegate.Broadcast(AnimOverride);
}

FS_AnimOverride_Movement ULocomotionSystem::GetCurrentOverrideStruct()
{
	FS_Anim_Movement CurrentMovement = GetCurrentMovementStruct();
    
	// Check if AnimOverrides map is empty
	if (CurrentMovement.AnimOverrides.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimOverrides map is empty"));
		return FS_AnimOverride_Movement();
	}

	FS_AnimOverride_Movement* L_AnimOverrideStruct = CurrentMovement.AnimOverrides.Find(AnimOverride);

	if (!L_AnimOverrideStruct)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimOverride '%s' not found, trying 'Default'"), *AnimOverride.ToString());
		L_AnimOverrideStruct = CurrentMovement.AnimOverrides.Find("Default");
        
		if (!L_AnimOverrideStruct)
		{
			UE_LOG(LogTemp, Error, TEXT("'Default' AnimOverride not found either! Returning empty struct"));
			return FS_AnimOverride_Movement();
		}
	}

	return *L_AnimOverrideStruct;
}


FS_Anim_Movement ULocomotionSystem::GetCurrentMovementStruct()
{
	return GetMovementFromMovementType(MovementType, GetAnimset(CurrentAnimsetRowName)->Movements);
}

FS_Anim_Movement ULocomotionSystem::GetMovementFromMovementType(EMovementType InMovementType, FS_Anim_MovementGroup Movements)
{
	return  InMovementType == EMovementType::Walk ? Movements.Walk :
		InMovementType == EMovementType::Jog ? Movements.Jog :
		InMovementType == EMovementType::Crouch ? Movements.Couch :
		FS_Anim_Movement();
}

UCharacterMovementComponent* ULocomotionSystem::GetMovementComponent() const
{
    if (!MovementComponent || !IsValid(MovementComponent))
    {
    	UE_LOG(LogTemp, Warning, TEXT("Getting MovementComponent from OwnerActor"));
        if (OwnerActor && IsValid(OwnerActor))
        {
        	UE_LOG(LogTemp, Warning, TEXT("OwnerActor is valid: %s"), *OwnerActor->GetName());
            const_cast<ULocomotionSystem*>(this)->MovementComponent = OwnerActor->GetCharacterMovement();
			UE_LOG(LogTemp, Warning, TEXT("MovementComponent: %s"), *MovementComponent->GetName());
        }
    }
    return MovementComponent;
}


void ULocomotionSystem::SetMaxWalkSpeed(float Speed)
{
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!MoveComp)
	{
		UE_LOG(LogTemp, Error, TEXT("SetMaxWalkSpeed: MovementComponent is null!"));
		return;
	}

	MoveComp->MaxWalkSpeed = Speed;
}

void ULocomotionSystem::SetMaxAcceleration(float Acceleration)
{
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!MoveComp)
	{
		UE_LOG(LogTemp, Error, TEXT("SetMaxWalkSpeed: MovementComponent is null!"));
		return;
	}

	MoveComp->MaxAcceleration = Acceleration;
}

void ULocomotionSystem::SetSprint(bool bSprint)
{
	IsSprinting = bSprint;
	SetMovementConfigs();
	OnSetSprintingDelegate.Broadcast(IsSprinting);
}

void ULocomotionSystem::SetCharacterBaseAimRotation()
{
	BaseAimRotation = OwnerActor->GetBaseAimRotation();
	
	FTimerHandle DelayTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [this]()
	{
		SetCharacterBaseAimRotation();
	}, 0.02f, false);
}

void ULocomotionSystem::SetRotationMode(ERotationMode NewRotationMode)
{
	RotationMode = NewRotationMode;
	if(MovementComponent)
	{
		MovementComponent->bOrientRotationToMovement = RotationMode == ERotationMode::ForwardFacing ? true : false;
		SetMovementConfigs();
	}
}

void ULocomotionSystem::SetRotationRate(const FRotator& NewRotationRate)
{
	MovementComponent->RotationRate = NewRotationRate;
	OnSetRotationModeDelegate.Broadcast(RotationMode);
}

void ULocomotionSystem::CheckAcceleration()
{
	if (HasAcceleration != GetIsAccelerating())
	{
		HasAcceleration = GetIsAccelerating();
		OnAccelerationChangedDelegate.Broadcast(HasAcceleration, GetLastVelocityDirection());
	}
}

bool ULocomotionSystem::GetIsAccelerating()
{
	if (MovementComponent)
	{
		return MovementComponent->GetCurrentAcceleration().Size2D() > 0.0f;
	}
	return false;
}

float ULocomotionSystem::GetLastVelocityDirection()
{
	return UKismetAnimationLibrary::CalculateDirection(MovementComponent->GetLastInputVector(), OwnerActor->GetActorRotation());
}
