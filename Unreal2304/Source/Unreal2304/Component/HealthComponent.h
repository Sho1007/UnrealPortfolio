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

private:
	void InitHealthArray();
	void CreateCapsules();

	void AddBleed();
	void SubBleed();
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void Bleeding();
private:
	TArray<TObjectPtr<UHealth>> HealthArray;

	uint8 BleedCount = 0;
	FTimerHandle BleedingTimerHandle;
};
