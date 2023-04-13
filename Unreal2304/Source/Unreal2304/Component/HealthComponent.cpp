// Fill out your copyright notice in the Description page of Project Settings.


#include "../Component/HealthComponent.h"
#include "../Component/Health.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	// ...
	Super::BeginPlay();
	InitHealthArray();
	CreateCapsules();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// ...
}

void UHealthComponent::InitHealthArray()
{
	for (uint8 i = 1; i < (uint8)EBodyType::Size; ++i)
	{
		HealthArray.Add(NewObject<UHealth>(this, UEnum::GetValueAsName((EBodyType)i)));
		HealthArray[i-1]->Init(this, UEnum::GetValueAsName((EBodyType)i), (EBodyType)i, 50, 50);
	}
}

void UHealthComponent::CreateCapsules()
{
	TObjectPtr<USkeletalMeshComponent> NewMesh = Cast<ACharacter>(GetOwner())->GetMesh();
	HealthArray[StaticCast<uint8>(EBodyType::Head) - 1]->AddCapsule(NewMesh, 12.5f, 9.0f, "head", FTransform(FRotator(90, 0, 0), FVector(5, 3, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::Thorax) - 1]->AddCapsule(NewMesh, 23.0f, 17.0f, "spine_03", FTransform(FRotator(90, 0, 0), FVector(-5, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::Stomach) - 1]->AddCapsule(NewMesh, 15.0f, 15.0f, "spine_01", FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::RightArm) - 1]->AddCapsule(NewMesh, 20.0f, 7.0f, "upperarm_r", FTransform(FRotator(90, 0, 0), FVector(-17, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::RightArm) - 1]->AddCapsule(NewMesh, 13.0f, 5.0f, "lowerarm_r", FTransform(FRotator(90, 0, 0), FVector(-17, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::LeftArm) - 1]->AddCapsule(NewMesh, 20.0f, 7.0f, "upperarm_l", FTransform(FRotator(90, 0, 0), FVector(17, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::LeftArm) - 1]->AddCapsule(NewMesh, 13.0f, 5.0f, "lowerarm_l", FTransform(FRotator(90, 0, 0), FVector(17, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::RightLeg) - 1]->AddCapsule(NewMesh, 25.0f, 9.0f, "thigh_r", FTransform(FRotator(90, 0, 0), FVector(18, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::RightLeg) - 1]->AddCapsule(NewMesh, 21.0f, 6.0f, "calf_r", FTransform(FRotator(90, 0, 0), FVector(19, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::RightLeg) - 1]->AddCapsule(NewMesh, 15.0f, 5.0f, "ball_r", FTransform(FRotator(90, 20, 0), FVector(10, 2, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::LeftLeg) - 1]->AddCapsule(NewMesh, 25.0f, 9.0f, "thigh_l", FTransform(FRotator(90, 0, 0), FVector(-18, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::LeftLeg) - 1]->AddCapsule(NewMesh, 21.0f, 6.0f, "calf_l", FTransform(FRotator(90, 0, 0), FVector(-19, 0, 0), FVector(1, 1, 1)));
	HealthArray[StaticCast<uint8>(EBodyType::LeftLeg) - 1]->AddCapsule(NewMesh, 15.0f, 5.0f, "ball_l", FTransform(FRotator(90, 20, 0), FVector(-10, -2, 0), FVector(1, 1, 1)));
}
