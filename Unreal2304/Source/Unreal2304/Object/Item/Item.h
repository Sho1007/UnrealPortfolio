// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Interface/Interactive.h"
#include "Item.generated.h"

/*
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Equipment,
	Usable,
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
	
	FText Name;
	FName DisplayName;
	EItemType ItemType;
	float Weight;
	FIntPoint Size;
	uint32 TargetIndex;
	TObjectPtr<UTexture2D> ItemImage;
	TSubclassOf<AItem> ItemClass;
};

// 아이템의 개별 정보
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	uint32 ID;
	FIntPoint InventoryPos;
	EItemState ItemState;
};

UCLASS()
class UNREAL2304_API AItem : public AActor, public IInteractive
{
	GENERATED_BODY()

public:
	// Inherited via IInteractive
	virtual void Interact(TObjectPtr<AActor> Character, uint8 SelectNum) override {}
	virtual TArray<FText>& GetMenuText() override;

	EItemState GetItemState() { return ItemData.ItemState; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FItemData ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FText> MenuText;
};
