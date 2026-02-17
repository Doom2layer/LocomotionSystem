// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Components/SubSystems/ActorProfileSystem.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "MainContent/LocomotionSystem/Data/SenseRules_Enum.h"
#include "MainContent/LocomotionSystem/Utilities/UtilitiesFunctionLibrary.h"
#include "MainContent/LocomotionSystem/Widget/ActorProfileContainer.h"
#include "MainContent/LocomotionSystem/Widget/Health.h"
#include "MainContent/LocomotionSystem/Widget/Team.h"

// Sets default values for this component's properties
UActorProfileSystem::UActorProfileSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UActorProfileSystem::BeginPlay()
{
    Super::BeginPlay();

    Owner = GetOwner();
    Character = Cast<ACharacter>(Owner);

	Owner->OnTakePointDamage.AddDynamic(this, &UActorProfileSystem::OnTakeDamage);
	
    InitializeActorProfileContainer();

    if (ActorProfileContainerWidget)
    {
        InitializeHealth();
        
        // Handle different sense rules at startup
        switch (SenseRule)
        {
        case ESenseRule::AlwaysActive:
            SetActorProfileContainerVisibility(ESlateVisibility::Visible);
            break;
            
        case ESenseRule::AlwaysInactive:
            SetActorProfileContainerVisibility(ESlateVisibility::Hidden);
            break;
            
        case ESenseRule::ActiveOnlyIfPlayerInRange:
            SetActorProfileContainerVisibility(ESlateVisibility::Hidden);
            break;
        }
    }
	
	InitializeEffects();
	InitializeTeam();
	
    // Only players trace for NPCs
    if (UUtilitiesFunctionLibrary::IsPlayer(Owner.Get()))
    {
        GetWorld()->GetTimerManager().SetTimer(
            TraceNPCTimer,
            this,
            &UActorProfileSystem::PerformNPCTrace,
            0.2f,
            true
        );
    }
	
	if (bShowTeam)
	{
		UTeam* LTeamWidget = CreateWidget<UTeam>(GetWorld(), TeamWidgetClass);
		LTeamWidget->SetTeamConfig(TeamConfig.TeamColor, TeamConfig.DisplayName);
		ActorProfileContainerWidget->AddElement(LTeamWidget);
	}
}

void UActorProfileSystem::PerformNPCTrace()
{
    // Get current NPCs in range
    TArray<AActor*> CurrentNPCs = TraceForNPCs();
    TArray<TWeakObjectPtr<AActor>> CurrentVisibleNPCs;


    // Process NPCs in range
    for (AActor* Actor : CurrentNPCs)
    {
        if (Actor)
        {
            UActorProfileSystem* System = UUtilitiesFunctionLibrary::GetActorProfileSystem(Actor);
            if (System)
            {
                ESenseRule NPCRule = System->GetSenseRule();
                
                // Only manage NPCs with ActiveOnlyIfPlayerInRange rule
                if (NPCRule == ESenseRule::ActiveOnlyIfPlayerInRange)
                {
                    System->SetActorProfileContainerVisibility(ESlateVisibility::Visible);
                    CurrentVisibleNPCs.Add(Actor);
                }
                // AlwaysActive NPCs are managed by themselves, don't interfere
                else if (NPCRule == ESenseRule::AlwaysActive)
                {
                }
            }
        }
    }

    for (TWeakObjectPtr<AActor>& PrevNPC : PreviouslyVisibleNPCs)
    {
        if (PrevNPC.IsValid())
        {
            bool bStillVisible = CurrentVisibleNPCs.ContainsByPredicate([&](const TWeakObjectPtr<AActor>& CurrentNPC)
            {
                return CurrentNPC.IsValid() && CurrentNPC.Get() == PrevNPC.Get();
            });

            if (!bStillVisible)
            {
                UActorProfileSystem* System = UUtilitiesFunctionLibrary::GetActorProfileSystem(PrevNPC.Get());
                if (System)
                {
                    if (System->GetSenseRule() == ESenseRule::ActiveOnlyIfPlayerInRange)
                    {
                        System->SetActorProfileContainerVisibility(ESlateVisibility::Hidden);
                    }
                }
            }
        }
    }

    PreviouslyVisibleNPCs = CurrentVisibleNPCs;
}

void UActorProfileSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UActorProfileSystem::InitializeHealth()
{
    
	if (!bShowHealthBar) return;
	if (!ActorProfileContainerWidget) return;
	if (!HealthWidgetClass) return;
	

	TObjectPtr<UHealth> HealthWidget = CreateWidget<UHealth>(GetWorld(), HealthWidgetClass);
	if (HealthWidget)
	{
		HealthWidget->SetOwner(Owner);
		HealthWidget->SetMaxHealth(MaxHealth);
		ActorProfileContainerWidget->AddElement(HealthWidget);
	}
}

void UActorProfileSystem::InitializeActorProfileContainer()
{
	if (!ActorProfileContainerClass)
	{
		return;
	}

	TObjectPtr<UWidgetComponent> L_WidgetComponent = Cast<UWidgetComponent>(Owner->AddComponentByClass(UWidgetComponent::StaticClass(), false, FTransform::Identity, false));
	
	if (!L_WidgetComponent)
	{
		return;
	}
	
	L_WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	L_WidgetComponent->SetDrawAtDesiredSize(true);
	L_WidgetComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ActorProfileContainerSocket);
	L_WidgetComponent->AddLocalOffset(ActorProfileContainerOffset);

	ActorProfileContainerWidget = CreateWidget<UActorProfileContainer>(GetWorld(), ActorProfileContainerClass);
    
	if (!ActorProfileContainerWidget)
	{
		return;
	}
    
	L_WidgetComponent->SetWidget(ActorProfileContainerWidget);
	L_WidgetComponent->RegisterComponent();
}

void UActorProfileSystem::InitializeTeam()
{
	if (!IsValid(TeamConfigDataTable))
	{
		return;
	}

	TeamConfig = GetTeamConfigsByTeamID(TeamID);
	if (bApplyTeamColor)
	{
		UpdateColorMeshColor(TeamConfig.TeamColor);
	}
}

FS_TeamConfig UActorProfileSystem::GetTeamConfigsByTeamID(ETeam InTeamID)
{
	// Check if data table is valid
	if (!IsValid(TeamConfigDataTable))
	{
		return FS_TeamConfig();
	}

	UDataTable* DataTable = TeamConfigDataTable.Get();
	if (!DataTable)
	{
		return FS_TeamConfig();
	}

	for (FName RowName : DataTable->GetRowNames())
	{
		FS_TeamConfig* FoundConfig = DataTable->FindRow<FS_TeamConfig>(RowName, FString());
		if (FoundConfig && FoundConfig->TeamID == InTeamID)
		{
			return *FoundConfig;
		}
	}

	return FS_TeamConfig();
}


void UActorProfileSystem::UpdateColorMeshColor(FLinearColor NewColor)
{
	if (Character)
	{
		if (Character->GetMesh())
		{
			USkeletalMeshComponent* Mesh = Character->GetMesh();
			int32 NumMaterials = Mesh->GetNumMaterials();
	 
			for (int32 Index = 0; Index < NumMaterials; ++Index)
			{
				UMaterialInterface* Material = Mesh->GetMaterial(Index);
				if (Material)
				{
					UMaterialInstanceDynamic* DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(Index, Material);
					if (DynamicMaterial)
					{
						DynamicMaterial->SetVectorParameterValue(TEXT("Tint"), NewColor);
					}
				}
			}
		}
	}
}

void UActorProfileSystem::SetActorProfileContainerVisibility(ESlateVisibility Visibility) const
{
	if (ActorProfileContainerWidget)
	{
		ActorProfileContainerWidget->SetVisibility(Visibility);
	}
}

void UActorProfileSystem::UpdateHealth(float Value)
{
	Health = FMath::Clamp(Health += Value, 0.0f, MaxHealth);
	OnHealthUpdated.Broadcast(Health);
}

TArray<AActor*> UActorProfileSystem::TraceForNPCs()
{
	TArray<AActor*> NPCList;
	
	TArray<FHitResult> HitResult;
	FVector Start = Character->GetActorLocation();
	FVector End = Start + FVector(0.0f, 0.0f, 0.1f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	
	GetWorld()->SweepMultiByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(SenseRange), QueryParams);
	
	for (const FHitResult& Hit : HitResult)
	{
		if (Hit.bBlockingHit)
		{
			if (UUtilitiesFunctionLibrary::IsNPC(Hit.GetActor(), "NPC"))
			{
				AActor* NPC = Hit.GetActor();
				NPCList.Add(NPC);	
			}
		}
	}
	return NPCList;
}

void UActorProfileSystem::InitializeEffects()
{
	if (!Effects.IsEmpty())
	{
		for (TSubclassOf<AActorProfileEffectsBase> Effect : Effects)
		{
			if (Effect)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
				SpawnParams.Owner = Owner.Get();
				SpawnParams.Instigator = Character.Get();
				TObjectPtr<AActorProfileEffectsBase> SpawnedEffect = GetWorld()->SpawnActor<AActorProfileEffectsBase>(Effect, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
				SpawnedEffect->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	}
}

void UActorProfileSystem::OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	OnTakeDamageDelegate.Broadcast(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	if (Health > 0.0f)
	{
		UpdateHealth(-Damage);
	}
	if (Health <= 0.0f)
	{
		OnDeathDelegate.Broadcast(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
		SetActorProfileContainerVisibility(ESlateVisibility::Hidden);
		SenseRule = ESenseRule::AlwaysInactive;
		bIsDead = true;
	}
}

ETeamAffiliation UActorProfileSystem::GetTeamAffiliation(AActor* TargetActor)
{
	ETeam LActorTeamID = UUtilitiesFunctionLibrary::GetActorProfileSystem(TargetActor)->GetTeamID();
	if (TeamConfig.Enemies.Contains(LActorTeamID))
	{
		return ETeamAffiliation::Enemy;
	}
	if (TeamConfig.Allies.Contains(LActorTeamID))
	{
		return ETeamAffiliation::Friendly;
	}
	return ETeamAffiliation::Neutral;
}
