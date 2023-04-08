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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 VerticalRecoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 HorizontalRecoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 EffectiveDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Ergonomics;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EFireMode> FireModes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RateOfFile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EBulletType Caliber;
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

	void ChangeFireMode();
private:
	void FireSingle();
	
private:
	// 개별 정보
	TObjectPtr<ACharacter> Owner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AMagazine> Magazine;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 CurrentFireMode;
	bool bCanFire = true;

	// 공통 정보
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> MuzzleFlash;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FName FirePosName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGunInfo GunInfo;

	// 단순 정보
	uint8 FireCount;
	FTimerHandle FireTimerHandle;
};
