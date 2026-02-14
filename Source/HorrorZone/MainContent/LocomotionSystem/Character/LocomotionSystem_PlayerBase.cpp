// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Character/LocomotionSystem_PlayerBase.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainContent/LocomotionSystem/Components/Helper/MontageHelper.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/WeaponSystem.h"

ALocomotionSystem_PlayerBase::ALocomotionSystem_PlayerBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ALocomotionSystem_PlayerBase::BeginPlay()
{
	Super::BeginPlay();
	if (MontageHelper)
	{
		MontageHelper->GetOnMontageCompleted().AddDynamic(this, &ALocomotionSystem_PlayerBase::OnMontageCompleted);
		MontageHelper->GetOnMontageNotifyBegin().AddDynamic(this, &ALocomotionSystem_PlayerBase::OnNotifyBegin);
	}
}

void ALocomotionSystem_PlayerBase::OnMontageCompleted(FName ID)
{
	UE_LOG(LogTemp, Warning, TEXT("Montage with ID '%s' completed."), *ID.ToString());
}

void ALocomotionSystem_PlayerBase::OnNotifyBegin(FName ID, FName NotifyName)
{
	UE_LOG(LogTemp, Warning, TEXT("Notify '%s' from montage with ID '%s' began."), *NotifyName.ToString(), *ID.ToString());
}

void ALocomotionSystem_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALocomotionSystem_PlayerBase::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALocomotionSystem_PlayerBase::Look);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::Walk);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &ALocomotionSystem_PlayerBase::Jog);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::MCrouch);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ALocomotionSystem_PlayerBase::UCrouch);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::Sprint);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ALocomotionSystem_PlayerBase::USprint);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::DJump);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ALocomotionSystem_PlayerBase::UJump);

		EnhancedInputComponent->BindAction(SwitchRotationAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::SwitchRotation);

		EnhancedInputComponent->BindAction(IncWeaponSlot, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::IncWeapon);

		EnhancedInputComponent->BindAction(DecWeaponSlot, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::DecWeapon);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::DFire);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ALocomotionSystem_PlayerBase::UFire);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ALocomotionSystem_PlayerBase::Reload);

		EnhancedInputComponent->BindAction(FirearmMeleeAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::DMelee);

		EnhancedInputComponent->BindAction(FirearmMeleeAction, ETriggerEvent::Completed, this, &ALocomotionSystem_PlayerBase::UMelee);
		
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ALocomotionSystem_PlayerBase::DAim);
		
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ALocomotionSystem_PlayerBase::UAim);
		
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ALocomotionSystem_PlayerBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ALocomotionSystem_PlayerBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ALocomotionSystem_PlayerBase::Walk(const FInputActionValue& Value)
{
	DoWalk();
}

void ALocomotionSystem_PlayerBase::Jog(const FInputActionValue& Value)
{
	DoJog();
}

void ALocomotionSystem_PlayerBase::MCrouch(const FInputActionValue& Value)
{
	DoCrouch();
}

void ALocomotionSystem_PlayerBase::UCrouch(const FInputActionValue& Value)
{
	ToggleCrouch(false);
}

void ALocomotionSystem_PlayerBase::Sprint(const FInputActionValue& Value)
{
	
	ToggleSprint(true);
}

void ALocomotionSystem_PlayerBase::USprint(const FInputActionValue& Value)
{
	ToggleSprint(false);
}

void ALocomotionSystem_PlayerBase::DJump(const FInputActionValue& Value)
{
	ToggleJump(true);
}

void ALocomotionSystem_PlayerBase::UJump(const FInputActionValue& Value)
{
	ToggleJump(false);
}

void ALocomotionSystem_PlayerBase::SwitchRotation(const FInputActionValue& Value)
{
	SwitchRotationMode(true);
}

void ALocomotionSystem_PlayerBase::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ALocomotionSystem_PlayerBase::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ALocomotionSystem_PlayerBase::DoWalk()
{
	ToggleWalk(true);
}

void ALocomotionSystem_PlayerBase::DoJog()
{
	ToggleWalk(false);
}

void ALocomotionSystem_PlayerBase::DoCrouch()
{
	ToggleCrouch(true);
}

void ALocomotionSystem_PlayerBase::DoUncrouch()
{
	ToggleCrouch(false);
}

void ALocomotionSystem_PlayerBase::OnOneKeyPressed()
{
	WeaponSystem->UseWeapon(1);
}

void ALocomotionSystem_PlayerBase::OnTwoKeyPressed()
{
	WeaponSystem->UseWeapon(0);
}

void ALocomotionSystem_PlayerBase::IncWeapon()
{
	WeaponSystem->IncrementSlot();
}

void ALocomotionSystem_PlayerBase::DecWeapon()
{
	WeaponSystem->DecrementSlot();
}

void ALocomotionSystem_PlayerBase::DFire(const FInputActionValue& Value)
{
	ToggleFire(true);
}

void ALocomotionSystem_PlayerBase::UFire(const FInputActionValue& Value)
{
	ToggleFire(false);
}

void ALocomotionSystem_PlayerBase::DMelee(const FInputActionValue& Value)
{
	ToggleMelee(true);
}

void ALocomotionSystem_PlayerBase::UMelee(const FInputActionValue& Value)
{
	ToggleMelee(false);
}

void ALocomotionSystem_PlayerBase::DAim(const FInputActionValue& Value)
{
	ToggleAim(true);
}

void ALocomotionSystem_PlayerBase::UAim(const FInputActionValue& Value)
{
	ToggleAim(false);
}
