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

private:
	bool EquipItem(TObjectPtr<AEquipment> Item);

// Gun
public:
	bool PickupItem(FItemData ItemData);
	bool EquipGun(TObjectPtr<AGun> GunActor);

	FVector GetZeroPointLocation();

private:
	void UpdateGunAttachment();
	TObjectPtr<AGun> EquippedGun();
	TObjectPtr<AGun> StowedGun();

	
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
	float ZeroPoint = 50.0f;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MenuBoxWidgetClass;
};
