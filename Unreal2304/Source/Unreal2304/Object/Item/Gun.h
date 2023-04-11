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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 VerticalRecoil = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HorizontalRecoil = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EffectiveDistance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Ergonomics = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EFireMode> FireModes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RateOfFire = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBulletType Caliber = EBulletType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> Sound_FireClose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> Sound_Loop_FireClose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> Sound_Loop_TailClose;
};


UCLASS()
class UNREAL2304_API AGun : public AEquipment
{
	GENERATED_BODY()

public:
	AGun();

	void BeginPlay() override;

	bool Fire();
	void FireStop();

	virtual void Interact(TObjectPtr<AActor> Character, uint8 SelectNum) override;

	void AttachToCharacter(TObjectPtr<ACharacter> Character, FName SocketName);
	void SetState(EItemState NewState);

	void ChangeFireMode();

	TObjectPtr<USkeletalMeshComponent> GetGunMesh() { return SkeletalMeshComponent; }
private:
	void PlayFireSound();
	void FireSingle();
	void ApplyRecoilToCharacter();
private:
	// 개별 정보
	TObjectPtr<ACharacter> Owner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AMagazine> Magazine;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 CurrentFireMode;
	bool bCanFire = true;
	bool bFired = false;

	// 공통 정보
	TObjectPtr<USceneComponent> BaseRootComponent;
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
	TObjectPtr<UAudioComponent> FireSoundComponent;
};
