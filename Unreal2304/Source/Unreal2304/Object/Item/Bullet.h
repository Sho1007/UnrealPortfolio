// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Item.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2304_API ABullet : public AItem
{
	GENERATED_BODY()
	
public:
	ABullet();
	void Fire(FTransform FireTransform);
private:
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> SphereComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Speed = 10000.0f;
};
