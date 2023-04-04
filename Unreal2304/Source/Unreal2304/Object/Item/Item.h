// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Interface/Interactive.h"
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

// 아이템의 공통 정보
USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()
protected:
	FString ItemName;
	TObjectPtr<UTexture2D> ItemImage;
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	TObjectPtr<USkeletalMesh> StaticMesh;
	FIntPoint ItemSize;
	EItemType ItemType;
};

// 아이템의 개별 정보
USTRUCT(BlueprintType)
struct FItemDataStruct
{
	GENERATED_BODY()

	int32 ItemIndex;
	FIntPoint InventoryPos;
};

UCLASS()
class UNREAL2304_API UItemData : public UObject
{
	GENERATED_BODY()
protected:
	int32 ItemIndex;
	FIntPoint InventoryPos;
	EItemState ItemState;
};

UCLASS()
class UNREAL2304_API AItem : public AActor, public IInteractive
{
	GENERATED_BODY()
	
public:
	// Inherited via IInteractive
	virtual void Interact(TObjectPtr<AMyCharacter> Character) override;
	virtual TArray<FText>& GetMenuText() override;

	// TArray<FItemData> Inventory;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UItemData> ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FText> MenuText;
};
