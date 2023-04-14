// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/MyCharacter.h"
#include "MyPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2304_API AMyPlayer : public AMyCharacter
{
	GENERATED_BODY()
	
public:
	AMyPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void CheckInteract();
	void CreateMenuBoxWidget(TObjectPtr<TArray<FText>> MenuText);
	void DeleteMenuBoxWidget();

	void InteractPressed();
	void WheelUp();
	void WheelDown();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void TurnRight(float Value);

	void CrouchPressed();

	void JumpPressed();
	void JumpReleased();

	void AttackPressed();
	void AttackReleased();
	void AimingPressed();

	void StopBreathPressed();
	void StopBreathReleased();

	void ChangeFireMode();
private:
	// Interact Var
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float InteractCheckRadius = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float InteractCheckLength = 120.0f;
	TObjectPtr<AActor> InteractActor;

	// Interact Widget Var
	TObjectPtr<UWidgetComponent> MenuBoxWidgetComponent;
	TObjectPtr<UMenuBoxWidget> MenuBoxWidget;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MenuBoxWidgetClass;
};
