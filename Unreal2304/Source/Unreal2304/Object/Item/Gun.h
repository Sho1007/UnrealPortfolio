// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Equipment.h"

#include "../Item/Magazine.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"

#include "Gun.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	None,
	Single,
	Burst2,
	Burst3,
	FullAuto,
	Size
};

USTRUCT(BlueprintType)
struct FGunInfo : public FTableRowBase
{
	GENERATED_BODY()
};


UCLASS()
class UNREAL2304_API AGun : public AEquipment
{
	GENERATED_BODY()

public:
	AGun();

	bool Fire();
	void FireStop();

	virtual void Interact(TObjectPtr<AActor> Character, uint8 SelectNum) override;

	void AttachToCharacter(TObjectPtr<ACharacter> Character, FName SocketName);
	void SetState(EItemState NewState);
private:
	void FireSingle();
	
private:
	// Owner
	TObjectPtr<ACharacter> Owner;
	// Component
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AMagazine> Magazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FName FirePosName;

	// Fire Mode
	bool bCanFire = true;
	uint8 FireCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<EFireMode> FireModes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 CurrentFireMode;

	FTimerHandle FireTimerHandle;
};
