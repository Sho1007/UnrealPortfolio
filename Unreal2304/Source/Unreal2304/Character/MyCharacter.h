// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "../Widget/MenuBoxWidget.h"
#include "../Object/Item/Gun.h"
#include "../Interface/Interactive.h"

#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class EGunSlot : uint8
{
	Primary,
	Secondary,
};

class UInventoryComponent;
class AScope;
class UHealthComponent;
UCLASS()
class UNREAL2304_API AMyCharacter : public ACharacter, public IInteractive
{
	GENERATED_BODY()

		friend class UHealth;
	friend class AMyPlayer;
	// Gun이 Character 의 private 에 접근할 필요가 있을까?
	//friend class AGun;
public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	bool EquipItem(TObjectPtr<AEquipment> Item);

	// Gun
public:
	bool PickupItem(FItemData ItemData);
	bool EquipGun(TObjectPtr<AGun> GunActor);
	bool EquipScope(TObjectPtr<AScope> NewScope);

	FVector GetZeroPointLocation();
	float GetZeroPoint() { return ZeroPoint; }
	void ApplyGunRecoil(int32 VerticalRecoil, int32 HorizontalRecoil);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		UCameraComponent* GetPlayerCamera() { return CameraComponent; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetAimButtonDown() { return bAimButtonDown; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetStopBreath() { return bStopBreath; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetCrouchButtonDown() { return bCrouchButtonDown; }
	void PlayGunFireAnimMontage(float Rate);

protected:
	void UpdateGunAttachment();
	TObjectPtr<AGun> EquippedGun();
	TObjectPtr<AGun> StowedGun();


	void AdjustGunRecoilFactor();


private:
	void Dead();

public:
	// Inherited via IInteractive
	virtual void Interact(TObjectPtr<AActor> Actor, uint8 SelectNum) override;
	virtual TObjectPtr<TArray<FText>> GetMenuText() override;

protected:
	// Component Var
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UHealthComponent> HealthComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;

		// Movement Var
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bJumpButtonDown = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bCrouchButtonDown = false;

	// Interact Var
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TArray<FText> MenuText;

protected:
	// Gun Var
	EGunSlot CurrentGunSlot = EGunSlot::Primary;
	TObjectPtr<AGun> PrimaryGun = NULL;
	TObjectPtr<AGun> SecondaryGun = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimMontage> FireHip_AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TObjectPtr<UAnimMontage> FireIronsight_AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float ZeroPoint = 50.0f;

protected:
	// Gun Recoil
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float GunRecoilFactor_Base = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float GunRecoilFactor_Min = 0.001f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float GunRecoilFactor_Adjust_Amount = 0.001f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float GunRecoilFactor_Adjust_Rate = 0.1f;

	float GunRecoilFactor = 0.01f;


	FTimerHandle GunRecoilTimerHandle;

	// Aiming Var
	bool bAimButtonDown = false;
	bool bStopBreath = false;
};
