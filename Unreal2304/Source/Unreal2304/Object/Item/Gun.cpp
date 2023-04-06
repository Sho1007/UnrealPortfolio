// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Gun.h"
#include "GameFramework/Character.h"

AGun::AGun() : AEquipment(EEquipmentType::Gun)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMeshComponent->SetCollisionProfileName("DroppedItem");
	SkeletalMeshComponent->SetSimulatePhysics(true);
}

bool AGun::Fire(FVector TargetLocation)
{
	if (Magazine == NULL || !Magazine->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Magazine"));
		return false;
	}

	FTransform FireTransform = SkeletalMeshComponent->GetSocketTransform(FirePosName);
	TObjectPtr<ABullet> Bullet = Magazine->Pop(FireTransform);
	if (Bullet == NULL || !Bullet->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("Magazine is empty"));
		return false;
	}

	// Todo : Add to Character [ZeroPoint]
	FVector FireDirection = TargetLocation - FireTransform.GetLocation();
	FireDirection.Normalize();
	Bullet->Fire(FireDirection);

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

void AGun::Interact(TObjectPtr<AActor> Character, uint8 SelectNum)
{
	TObjectPtr<IInteractive> Interface = Cast<IInteractive>(Character);
	if (Interface == NULL) return;
	switch (SelectNum)
	{
	case 0:
		Interface->Interact(this, (uint8)ECharacterInteract::PickupEquipment);
		break;
	}
}

void AGun::AttachToCharacter(TObjectPtr<ACharacter> Character, FName SocketName)
{
	FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh(), AttachmentRule, SocketName);

	UE_LOG(LogTemp, Warning, TEXT("AttachToCharacter Called"));
}

void AGun::SetState(EItemState NewState)
{
	ItemData.ItemState = NewState;
	switch (NewState)
	{
	case EItemState::Dropped:
		SkeletalMeshComponent->SetCollisionProfileName("DroppedItem");
		SkeletalMeshComponent->SetSimulatePhysics(true);
		break;
	case EItemState::Inventory:
		break;
	case EItemState::Equipped:
		SkeletalMeshComponent->SetSimulatePhysics(false);
		SkeletalMeshComponent->SetCollisionProfileName("NoCollision");
		break;
	}
}
