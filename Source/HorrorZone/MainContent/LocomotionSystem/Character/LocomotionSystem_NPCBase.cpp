// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Character/LocomotionSystem_NPCBase.h"
#include "MainContent/LocomotionSystem/AI/Controller/LSAIController.h"
#include "MainContent/LocomotionSystem/Components/SubSystems/NPCSystem.h"

ALocomotionSystem_NPCBase::ALocomotionSystem_NPCBase()
{
	Tags.Add("NPC");
	AIControllerClass = ALSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	NPCSystem = CreateDefaultSubobject<UNPCSystem>(TEXT("NPCSystem"));
}

void ALocomotionSystem_NPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALocomotionSystem_NPCBase::BeginPlay()
{
	Super::BeginPlay();
}

