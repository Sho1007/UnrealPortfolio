// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Gun.h"
#include "GameFramework/Character.h"
#include "../../Character/MyCharacter.h"

AGun::AGun() : AEquipment(EEquipmentType::Gun)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMeshComponent->SetCollisionProfileName("DroppedItem");
	SkeletalMeshComponent->SetSimulatePhysics(true);
}

bool AGun::Fire()
{
	if (!bCanFire) return false;

	switch (FireModes[CurrentFireMode])
	{
	case EFireMode::None:
		break;
	case EFireMode::Single:
		FireSingle();
		break;
	case EFireMode::Burst2:
		bCanFire = false;
		FireCount = 2;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 0.05f, true);
		break;
	case EFireMode::Burst3:
		bCanFire = false;
		FireCount = 3;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 0.05f, true);
		break;
	case EFireMode::FullAuto:
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 0.05f, true);
		break;
	case EFireMode::Size:
		break;
	default:
		break;
	}

	return true;
}

void AGun::FireSingle()
{
	if (Magazine == NULL || !Magazine->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Magazine"));
		FireStop();
		return;
	}
	FTransform FireTransform = SkeletalMeshComponent->GetSocketTransform(FirePosName);
	TObjectPtr<ABullet> Bullet = Magazine->Pop(FireTransform);
	if (Bullet == NULL || !Bullet->IsValidLowLevelFast())
	{
		FireStop();
		return;
	}

	if (FireModes[CurrentFireMode] == EFireMode::Burst2 || FireModes[CurrentFireMode] == EFireMode::Burst3)
	{
		if (--FireCount == 0)
		{
			FireStop();
		}
	}
	FVector FireDirection = Cast<AMyCharacter>(Owner)->GetZeroPointLocation() - Bullet->GetActorLocation();
	FireDirection.Normalize();

	Bullet->Fire(FireDirection);

	/*if (MuzzleFlash == NULL || !MuzzleFlash->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("MuzzleFlash is null or not valid"));
	}
	else
	{
		FActorSpawnParameters ActorSpawnParameters;
		GetWorld()->SpawnActor<AActor>(MuzzleFlash->StaticClass(), FireTransform, ActorSpawnParameters);
	}*/

	return;
}

void AGun::FireStop()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	bCanFire = true;
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
	if (Character == NULL || !Character->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, "Character is not valid");
		return;
	}
	Owner = Character;
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