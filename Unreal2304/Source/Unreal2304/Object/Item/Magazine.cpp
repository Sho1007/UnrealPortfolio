// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Magazine.h"

AMagazine::AMagazine() : AEquipment(EEquipmentType::Magazine)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
}

TObjectPtr<ABullet> AMagazine::Pop()
{
	if (IsEmpty()) return NULL;
	UE_LOG(LogTemp, Warning, TEXT("Pop Called Without IsEmpty"));

	TObjectPtr<ABullet> Bullet = Bullets[Bullets.Num() - 1];
	Bullets.RemoveAt(Bullets.Num() - 1);

	return Bullet;
}