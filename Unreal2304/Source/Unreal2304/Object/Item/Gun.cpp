// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Gun.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "../../Character/MyCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Components/AudioComponent.h"

AGun::AGun() : AEquipment(EEquipmentType::Gun)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetSimulatePhysics(true);
	SkeletalMeshComponent->SetCollisionProfileName("DroppedItem");
	FireSoundComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	FireSoundComponent->SetupAttachment(SkeletalMeshComponent, FirePosName);
	FireSoundComponent->Stop();
}

void AGun::BeginPlay()
{
}

bool AGun::Fire()
{
	if (!bCanFire) return false;
	if (CurrentFireMode >= GunInfo.FireModes.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "CurrentFireMode is not valid");
		return false;
	}

	switch (GunInfo.FireModes[CurrentFireMode])
	{
	case EFireMode::None:
		break;
	case EFireMode::Single:
		FireSingle();
		break;
	case EFireMode::Burst2:
		bCanFire = false;
		FireCount = 2;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 60.0f / GunInfo.RateOfFire, true);
		break;
	case EFireMode::Burst3:
		bCanFire = false;
		FireCount = 3;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 60.0f / GunInfo.RateOfFire, true);
		break;
	case EFireMode::FullAuto:
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 60.0f / GunInfo.RateOfFire, true);
		break;
	case EFireMode::Size:
		break;
	default:
		break;
	}

	return true;
}

void AGun::PlayFireSound()
{
	// Todo : Fire Sound

	switch (GunInfo.FireModes[CurrentFireMode])
	{
	case EFireMode::Single:
		UGameplayStatics::PlaySoundAtLocation(this, GunInfo.Sound_FireClose, SkeletalMeshComponent->GetSocketLocation(FirePosName));
		break;
	case EFireMode::Burst2:
	case EFireMode::Burst3:
	case EFireMode::FullAuto:
		if (!FireSoundComponent->IsPlaying())
		{
			UGameplayStatics::PlaySoundAtLocation(this, GunInfo.Sound_FireClose, SkeletalMeshComponent->GetSocketLocation(FirePosName));
			FireSoundComponent->SetSound(GunInfo.Sound_Loop_FireClose);
			FireSoundComponent->Play();
		}
		break;
	}

	if (GunInfo.Sound_FireClose == NULL || !GunInfo.Sound_FireClose->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Sound Fire is not valid");
		return;
	}
	//FireSoundComponent->Deactivate();
}

void AGun::FireSingle()
{
	if (Magazine == NULL || !Magazine->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "[FireSingle] Magazine is not valid");
		return;
	}

	FTransform FireTransform = SkeletalMeshComponent->GetSocketTransform(FirePosName);
	TObjectPtr<ABullet> Bullet = Magazine->Pop(FireTransform);
	if (Bullet == NULL || !Bullet->IsValidLowLevelFast())
	{
		FireStop();
		return;
	}

	if (Bullet->GetBulletType() != GunInfo.Caliber)
	{

	}

	if (GunInfo.FireModes[CurrentFireMode] == EFireMode::Burst2 || GunInfo.FireModes[CurrentFireMode] == EFireMode::Burst3)
	{
		if (--FireCount == 0)
		{
			FireStop();
		}
	}

	bFired = true;

	// Todo : Apply Reocil
	ApplyRecoilToCharacter();

	// Todo : Play Sound
	PlayFireSound();

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

void AGun::ApplyRecoilToCharacter()
{
	// Todo : Recoil 적용 방법이 맞는지 확인 후 재적용

	Cast<AMyCharacter>(Owner)->ApplyGunRecoil(-GunInfo.VerticalRecoil, GunInfo.HorizontalRecoil * (FMath::RandRange(0, 1) == 1 ? -1 : 1));
}

void AGun::FireStop()
{
	if (!bFired) return; bFired = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	bCanFire = true;
	if (FireSoundComponent == NULL || !FireSoundComponent->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "[FireStop] Sound Component is not valid");
		return;
	}
	if (GunInfo.Sound_Loop_TailClose == NULL || !GunInfo.Sound_Loop_TailClose->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "[FireStop] Sound_Loop_TailClose is not valid");
		return;
	}
	if (GunInfo.FireModes[CurrentFireMode] != EFireMode::Single)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunInfo.Sound_Loop_TailClose, SkeletalMeshComponent->GetSocketLocation(FirePosName));
	}
	FireSoundComponent->Stop();
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
	SetState(EItemState::Equipped);
	Owner = Character;
	SkeletalMeshComponent->SetSimulatePhysics(false);
	FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh(), AttachmentRule, SocketName);
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

void AGun::ChangeFireMode()
{
	CurrentFireMode = (CurrentFireMode + 1) % GunInfo.FireModes.Num();

	// Todo : [현재 발사 모드 화면]출력 기능 구현 필요
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, UEnum::GetValueAsString(GunInfo.FireModes[CurrentFireMode]));
}