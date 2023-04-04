// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Gun.h"

AGun::AGun() : AEquipment(EEquipmentType::Gun)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);
}

bool AGun::Fire()
{
	if (Magazine == NULL || !Magazine->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Magazine"));
		return false;
	}

	TObjectPtr<ABullet> Bullet = Magazine->Pop();
	if (Bullet == NULL || !Bullet->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("Magazine is empty"));
		return false;
	}

	FName FirePosName;
	FTransform FireTransform = SkeletalMeshComponent->GetSocketTransform(FirePosName);
	Bullet->Fire(FireTransform.GetLocation(), FireTransform.GetRotation().GetForwardVector());

	if (MuzzleFlash == NULL || !MuzzleFlash->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("MuzzleFlash is null or not valid"));
		return false;
	}
	else
	{
		FActorSpawnParameters ActorSpawnParameters;
		GetWorld()->SpawnActor<AActor>(MuzzleFlash->StaticClass(), FireTransform, ActorSpawnParameters);
	}

	return true;
}
