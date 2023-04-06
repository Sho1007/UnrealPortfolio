// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Equipment.h"

#include "../Item/Magazine.h"
#include "Components/BoxComponent.h"

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

	bool Fire(FVector TargetLocation);

	virtual void Interact(TObjectPtr<AActor> Character, uint8 SelectNum) override;

	void AttachToCharacter(TObjectPtr<ACharacter> Character, FName SocketName);
	void SetState(EItemState NewState);
private:
	// Component
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AMagazine> Magazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FName FirePosName;
};
