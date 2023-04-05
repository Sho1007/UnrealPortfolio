// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Equipment.h"
#include "../Item/Bullet.h"
#include "Magazine.generated.h"

/**
 * 
 */

UCLASS()
class UNREAL2304_API AMagazine : public AEquipment
{
	GENERATED_BODY()
	
public:
	AMagazine();
	bool IsEmpty() { return Bullets.Num() == 0; }
	TObjectPtr<ABullet> Pop();
private:
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	TArray<int8> BulletDataArray;
	TArray<TObjectPtr<ABullet>> Bullets;
};
