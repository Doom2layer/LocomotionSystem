// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/LocomotionSystem/Widget/ActorProfileContainer.h"

#include "Components/VerticalBox.h"

void UActorProfileContainer::AddElement(UUserWidget* Widget) const
{
	if (!VBContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("VBContainer is null! Make sure you're using a Widget Blueprint with a VerticalBox named 'VBContainer'"));
		return;
	}
	
	if (!Widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to add null widget to ActorProfileContainer"));
		return;
	}
	
	VBContainer->AddChild(Widget);
}
