// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"

#include "MyCharacter.generated.h"

UCLASS()
class UNREAL2304_API AMyCharacter : public ACharacter
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
	void CreateMenuBoxWidget();
	void DeleteMenuBoxWidget();

// Movement Input Bind
private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void TurnRight(float Value);


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
	float InteractCheckRadius = 5.0f;
	float InteractCheckLength = 100.0f;
	TObjectPtr<AActor> InteractActor;

	// Interact Widget Var
	TObjectPtr<UWidgetComponent> MenuBoxWidgetComponent;
	TObjectPtr<UUserWidget> MenuBoxWidget;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MenuBoxWidgetClass;
};
