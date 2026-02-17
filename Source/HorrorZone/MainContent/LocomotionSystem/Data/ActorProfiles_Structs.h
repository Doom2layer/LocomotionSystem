// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorProfiles_Enums.h"
#include "ActorProfiles_Structs.generated.h"

enum class ETeam : uint8;
/**
 * 
 */
USTRUCT(BlueprintType, meta=(DisplayName="S Team Config"))
struct HORRORZONE_API FS_TeamConfig : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team Config")
	ETeam TeamID = ETeam::Blue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team Config")
	FName DisplayName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team Config")
	FLinearColor TeamColor = FLinearColor::Blue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team Config")
	TArray<ETeam> Enemies;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team Config")
	TArray<ETeam> Allies;
};
