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

	// ���Ӹ��� dATA
	// ������ DATA
	// źâ DATA
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
