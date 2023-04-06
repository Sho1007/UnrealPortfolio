// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Item.h"

#include "Bullet.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	None,
	Rifle_556x45,
	Size
};

USTRUCT(BlueprintType)
struct FBulletInfo
{
	GENERATED_BODY()
	EBulletType BulletType;
	uint32 Speed;
	uint8 Damage;
	uint8 PenetrationPower;
	float ArmorDamage;
	float Accuracy;
	int8 Recoil;
	float FragChance;
	float LightBleedChance;
	float HeavyBleedChance;
	TArray<uint8> BulletEffectivenessAgainstArmorClass[7];
};

UCLASS()
class UNREAL2304_API ABullet : public AItem
{
	GENERATED_BODY()
	
public:
	ABullet();
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void Fire(FVector MoveDirection);
private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> CollisionComponent;
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};