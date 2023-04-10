// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../../Interface/Interactive.h"
#include "Engine/DataTable.h"

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
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = FText();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Size = FIntPoint();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	AItem();
	// Inherited via IInteractive
	virtual void Interact(TObjectPtr<AActor> Character, uint8 SelectNum) override {}
	virtual TArray<FText>& GetMenuText() override;

	EItemState GetItemState() { return ItemData.ItemState; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FItemData ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FText> MenuText;
};
