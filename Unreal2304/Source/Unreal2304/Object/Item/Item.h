// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Item.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Equipment,
	Consumable,
	Ingredient,
	Size
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None,
	Dropped,
	Inventory,
	Equipped,
	Size
};

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY();

	FString ItemName;
	TObjectPtr<UTexture2D> ItemImage;
	TObjectPtr<UMeshComponent> ItemMesh;
	FIntPoint ItemSize;
	EItemType ItemType;
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY();

	int ItemIndex;
	FIntPoint InventoryPos;
	EItemState ItemState;
	TArray<int32> IntArray;
	TArray<float> FloatArray;
	TArray<bool> BoolArray;
};

UCLASS()
class UNREAL2304_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FItemData ItemData;
};
