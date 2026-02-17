// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/Team.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void UTeam::SetTeamConfig(FLinearColor Color, FName TeamName)
{
	if (TeamColor)
	{
		TeamColor->SetBrushColor(Color);
	}
	if (TeamNameText)
	{
		TeamNameText->SetText(FText::FromName(TeamName));
	}
}
