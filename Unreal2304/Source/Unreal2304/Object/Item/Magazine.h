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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Capacity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Ergonomics = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LoadUnloadSpeedModifier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CheckSpeedModifier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBulletType Caliber = EBulletType::None;
};

USTRUCT(BlueprintType)
struct FMagazineData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> BulletStack;
};

UCLASS()
class UNREAL2304_API AMagazine : public AEquipment
{
	GENERATED_BODY()
	
public:
	AMagazine();
	bool IsEmpty()
	{
		return MagazineData.BulletStack.Num() == 0;
	}
	TObjectPtr<ABullet> Pop(FTransform FireTransform);
	bool Push(int32 BulletID);
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> BulletClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FMagazineInfo MagazineInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FMagazineData MagazineData;
};
