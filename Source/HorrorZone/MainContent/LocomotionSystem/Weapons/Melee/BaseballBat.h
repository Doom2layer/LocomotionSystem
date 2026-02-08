// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeBase.h"
#include "BaseballBat.generated.h"

UCLASS()
class HORRORZONE_API ABaseballBat : public AMeleeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseballBat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
