// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealth;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL2304_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

friend class UHealth;
public:	
	// Sets default values for this component's properties
	UHealthComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	bool CheckIsDead() { return bIsDead; }
private:
	void InitHealthArray();
	void CreateCapsules();

	void Dead();

	void DesperseDamage(float NewDamage, float DesperseFactor);

	void AddLightBleed();
	void SubLightBleed();
	void AddHeavyBleed();
	void SubHeavyBleed();
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void LightBleeding();
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void HeavyBleeding();
private:
	TArray<TObjectPtr<UHealth>> HealthArray;

	bool bIsDead = false;

	uint8 LightBleedCount = 0;
	FTimerHandle LightBleedingTimerHandle;
	uint8 HeavyBleedCount = 0;
	FTimerHandle HeavyBleedingTimerHandle;
};
