// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Equipment.h"
#include "../Item/Bullet.h"
#include "Magazine.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMagazineInfo
{
	GENERATED_BODY()
	uint8 Capacity;
	uint8 Ergonomics;
	int8 LoadUnloadSpeedModifier;
	int8 CheckSpeedModifier;
	EBulletType Caliber;
};

USTRUCT(BlueprintType)
struct FMagazineData
{
	GENERATED_BODY()
	TArray<uint32> BulletStack;
};

UCLASS()
class UNREAL2304_API AMagazine : public AEquipment
{
	GENERATED_BODY()
	
public:
	AMagazine();
	bool IsEmpty()
	{
		return false;
	}
	TObjectPtr<ABullet> Pop(FTransform FireTransform);
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABullet> BulletClass;
};
