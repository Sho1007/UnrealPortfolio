// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Interactive.generated.h"

UENUM(BlueprintType)
enum class ECharacterInteract : uint8
{
	None,
	PickupItem,
	PickupEquipment,
	Size
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL2304_API IInteractive
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(TObjectPtr<AActor> Actor, uint8 SelectNum) = 0;
	virtual TObjectPtr<TArray<FText>> GetMenuText() = 0;
};
