// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Magazine.h"

AMagazine::AMagazine() : AEquipment(EEquipmentType::Magazine)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
}

TObjectPtr<ABullet> AMagazine::Pop(FTransform FireTransform)
{
	if (BulletClass == NULL || !BulletClass->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString("Bullet Class is Not Valid"));
		return NULL;
	}
	if (IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Magazine is empty"));
		return NULL;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, FString::Printf(TEXT("%d"), MagazineData.BulletStack.Last()));
	MagazineData.BulletStack.Pop();

	// DataTable 에서 Class 가져와서 스폰하는걸로 바꾸기
	return GetWorld()->SpawnActor<ABullet>(BulletClass, FireTransform);
}