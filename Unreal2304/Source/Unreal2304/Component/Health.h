// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Health.generated.h"

/**
 * 
 */
UENUM (BlueprintType)
enum class EStatusType : uint8
{
	LightBleeding,
	HeavyBleeding,
	FreshWound,
	Fracture,
	Pain,
	MildMusclePain,
	ServerMusclePain,
	Concussion,
	Tremor,
	TunnelVision,
	OnPainKiller,
	Dehydration,
	HardDehydration,
	Fatigue,
	HardFatigue,
	OverWeight,
	CriticalOverWeight,
	FatigueOverWeight,
	Stun,
	Flash,
	Buff,
	Debuff,
	Berserk,
	Poisoning,
	Size,
};

UENUM(BlueprintType)
enum class EBodyType : uint8
{
	None,

	Head,
	Thorax,
	Stomach,
	RightArm,
	LeftArm,
	RightLeg,
	LeftLeg,

	Size,
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None,

	Physics,
	Bleed,
	Biology,

	Size,
};

class UCapsuleComponent;
class UHealthComponent;
UCLASS()
class UNREAL2304_API UHealth : public UObject
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr<UHealthComponent> NewOwner, FName NewName, EBodyType NewBodyType, float NewHealthCurrent, float NewHealthMax);
	void AddCapsule(TObjectPtr<USkeletalMeshComponent> NewMesh, float NewHalfHeight, float NewRadius, FName NewSocketName, FTransform NewRelativeTransform);
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BleedFunction();
private:
	void Bleed();
	
	void BlackOut();
	void ApplyDamage(float NewDamage, EDamageType NewDamageType);
private:
	FName Name;
	EBodyType BodyType;
	float HealthCurrent;
	float HealthMax;
	TArray<TObjectPtr<UCapsuleComponent>> CapsuleArray;
	TArray<bool> StatusArray;
	TObjectPtr<UHealthComponent> Owner;

	// Bleed
	bool bIsBleed;
	FTimerHandle BleedTimerHandle;
};