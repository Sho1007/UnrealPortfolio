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

	FString ItemName;
	FIntPoint ItemSize;
	TObjectPtr<UTexture2D> ItemImage;
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	TObjectPtr<USkeletalMesh> StaticMesh;
	TSubclassOf<AItem> ItemClass;
	EItemType ItemType;
};

// 아이템의 개별 정보
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	int32 ItemIndex;
	FIntPoint InventoryPos;
	EItemState ItemState;

	TArray<int32> IntArray;
	TArray<float> FloatArray;
	TArray<bool> BoolArray;

	TArray<FItemData> ChildData;
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
