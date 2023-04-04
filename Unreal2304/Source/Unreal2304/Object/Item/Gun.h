// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Equipment.h"
#include "Gun.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FGunData
{
	GENERATED_BODY()

	// 개머리판 dATA
	// 손잡이 DATA
	// 탄창 DATA
};

UCLASS()
class UNREAL2304_API AGun : public AEquipment
{
	GENERATED_BODY()

public:
	AGun();
private:
	// Component
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
};
