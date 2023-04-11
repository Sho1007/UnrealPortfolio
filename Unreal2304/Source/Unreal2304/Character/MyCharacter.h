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

UCLASS()
class UNREAL2304_API AMyCharacter : public ACharacter, public IInteractive
{
	GENERATED_BODY()
	
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
	void CheckInteract();
	void CreateMenuBoxWidget(TArray<FText>& MenuText);
	void DeleteMenuBoxWidget();

// Movement Input Bind
private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void TurnRight(float Value);

	void WheelUp();
	void WheelDown();

	void Crouch();
	void JumpPressed();
	void JumpReleased();

	void InteractPressed();

	void AttackPressed();
	void AttackReleased();

	void AimingPressed();

	void StopBreathPressed();
	void StopBreathReleased();

	void ChangeFireMode();

	

private:
	bool EquipItem(TObjectPtr<AEquipment> Item);

// Gun
public:
	bool PickupItem(FItemData ItemData);
	bool EquipGun(TObjectPtr<AGun> GunActor);

	FVector GetZeroPointLocation();
	void ApplyGunRecoil(int32 VerticalRecoil, int32 HorizontalRecoil);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCameraComponent* GetPlayerCamera() { return CameraComponent; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetAimButtonDown() { return bAimButtonDown; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetStopBreath() { return bStopBreath; }
	void PlayGunFireAnimMontage(float Rate);

private:
	void UpdateGunAttachment();
	TObjectPtr<AGun> EquippedGun();
	TObjectPtr<AGun> StowedGun();

	void AdjustGunRecoilFactor();

	
public:
	// Inherited via IInteractive
	virtual void Interact(TObjectPtr<AActor> Actor, uint8 SelectNum) override;
	virtual TArray<FText>& GetMenuText() override;

private:
	// Component Var
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;

	// Movement Var
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bJumpButtonDown = false;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bCrouchButtonDown = false;

	// Interact Var
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float InteractCheckRadius = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float InteractCheckLength = 120.0f;
	TObjectPtr<AActor> InteractActor;
	TArray<FText> MenuText;

	// Interact Widget Var
	TObjectPtr<UWidgetComponent> MenuBoxWidgetComponent;
	TObjectPtr<UMenuBoxWidget> MenuBoxWidget;

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
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MenuBoxWidgetClass;
};
