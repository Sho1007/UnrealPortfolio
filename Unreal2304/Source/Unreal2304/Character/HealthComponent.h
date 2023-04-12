// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EBodyType : uint8
{
	None,
	Head,
	Thorax,
	Stomach,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
	RightThigh,
	RightCalf,
	RightBall,
	LeftThigh,
	LeftCalf,
	LeftBall,
	Size,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL2304_API UHealthComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void CreateCapsule(float NewHalfHeight, float NewRadius, FName NewSocketName, FTransform NewRelativeTransform);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void CapusleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	TArray<TObjectPtr<class UCapsuleComponent>> CapsuleArray;
};
