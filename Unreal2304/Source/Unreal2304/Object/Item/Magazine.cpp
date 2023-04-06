// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Magazine.h"

AMagazine::AMagazine() : AEquipment(EEquipmentType::Magazine)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
}

TObjectPtr<ABullet> AMagazine::Pop(FTransform FireTransform)
{

	/*if (IsEmpty()) return NULL;
	UE_LOG(LogTemp, Warning, TEXT("Pop Called Without IsEmpty"));

	TObjectPtr<ABullet> Bullet = Bullets[Bullets.Num() - 1];
	Bullets.RemoveAt(Bullets.Num() - 1);*/

	if (BulletClass == NULL || !BulletClass->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("Bullet Class is Not Valid"));
		return NULL;
	}

	return GetWorld()->SpawnActor<ABullet>(BulletClass, FireTransform);
}