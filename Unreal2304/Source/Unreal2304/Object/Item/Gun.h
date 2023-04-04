// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Equipment.h"
#include "../Item/Magazine.h"
#include "Gun.generated.h"

/**
 * 
 */

UCLASS()
class UNREAL2304_API AGun : public AEquipment
{
	GENERATED_BODY()

public:
	AGun();

	bool Fire();

private:
	// Component
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AMagazine> Magazine = NULL;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> MuzzleFlash;
};
