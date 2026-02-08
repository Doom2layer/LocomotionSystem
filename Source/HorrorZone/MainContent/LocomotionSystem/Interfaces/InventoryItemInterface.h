// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInterface.generated.h"

/**
* Empty class for reflection system visibility.
* Uses the UINTERFACE macro.
* Inherits from UInterface.
*/
UINTERFACE()
class UInventoryItemInterface : public UInterface
{
	GENERATED_BODY()
};

class IInventoryItemInterface
{
	GENERATED_BODY()

public:
	virtual void EquipItem();

	virtual void UnequipItem();
};
