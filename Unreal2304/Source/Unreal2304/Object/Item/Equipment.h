// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Item.h"
#include "Equipment.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	None,
	Gun,
	HeadGear,
	HeadSet,
	Face,
	Rig,
	Armor,
	ArmorRig,
	Magazine,
	Size
};

UCLASS()
class UNREAL2304_API AEquipment : public AItem
{
	GENERATED_BODY()
public:
	AEquipment() {}
	AEquipment(EEquipmentType Type);

	EEquipmentType GetEquipmentType() { return EquipmentType; }
protected:
	EEquipmentType EquipmentType;
};
