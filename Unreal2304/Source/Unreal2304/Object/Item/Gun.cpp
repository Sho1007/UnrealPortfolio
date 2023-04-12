// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Gun.h"
#include "../Item/Scope.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "../../Character/MyCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Components/AudioComponent.h"

AGun::AGun() : AEquipment(EEquipmentType::Gun)
{
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
	Super::BeginPlay();
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
		FireCount = 1;
		FireSingle();
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 60.0f / GunInfo.RateOfFire, true, 60.0f / GunInfo.RateOfFire);
		break;
	case EFireMode::Burst3:
		bCanFire = false;
		FireCount = 2;
		FireSingle();
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 60.0f / GunInfo.RateOfFire, true, 60.0f / GunInfo.RateOfFire);
		break;
	case EFireMode::FullAuto:
		FireSingle();
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGun::FireSingle, 60.0f / GunInfo.RateOfFire, true, 60.0f / GunInfo.RateOfFire);
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
}

void AGun::FireSingle()
{
	bFired = true;
	if (Magazine == NULL || !Magazine->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "[FireSingle] Magazine is not valid");
		return;
	}
	FTransform FireTransform = SkeletalMeshComponent->GetSocketTransform(FirePosName);
	
	//FireTransform.SetLocation(FireTransform.GetLocation() + (FireDirection * 20.0f));
	TObjectPtr<ABullet> Bullet = Magazine->Pop(FireTransform);
	if (Bullet == NULL || !Bullet->IsValidLowLevelFast())
	{
		FireStop();
		return;
	}
	Bullet->SetInstigator(Owner);

	if (Bullet->GetBulletType() != GunInfo.Caliber)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "[FireSingle] Bullet Type is not Matched");
		return;
	}

	FVector FireDirection;
	if (Scope != NULL && Scope->IsValidLowLevelFast())
	{
		FireDirection = Scope->GetActorLocation() + (Cast<AMyCharacter>(Owner)->GetZeroPoint() * -100 * Scope->GetActorForwardVector());
	}
	else
	{
		FireDirection = Cast<AMyCharacter>(Owner)->GetZeroPointLocation();
	}
	FireDirection -= FireTransform.GetLocation();
	FireDirection.Normalize();

	Bullet->Fire(FireDirection);
	

	if (GunInfo.FireModes[CurrentFireMode] == EFireMode::Burst2 || GunInfo.FireModes[CurrentFireMode] == EFireMode::Burst3)
	{
		if (--FireCount == 0)
		{
			FireStop();
		}
	}

	// Todo : Apply Reocil
	ApplyRecoilToCharacter(); 

	// Todo : Play Sound
	PlayFireSound();

	Cast<AMyCharacter>(Owner)->PlayGunFireAnimMontage(60.0f / GunInfo.RateOfFire);

	if (MuzzleFlash == NULL || !MuzzleFlash->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("MuzzleFlash is null or not valid"));
	}
	else
	{
		FActorSpawnParameters ActorSpawnParameters;
		GetWorld()->SpawnActor<AActor>(MuzzleFlash, FireTransform, ActorSpawnParameters);
	}

	return;
}

void AGun::ApplyRecoilToCharacter()
{
	// Todo : Recoil 적용 방법이 맞는지 확인 후 재적용

	Cast<AMyCharacter>(Owner)->ApplyGunRecoil(-GunInfo.VerticalRecoil, GunInfo.HorizontalRecoil * (FMath::RandRange(0, 1) == 1 ? -1 : 1));
}

void AGun::FireStop()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "[FireStop] Function Called");
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	
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
	FireSoundComponent->Stop();
	if (!bFired) return; bFired = false;
	bCanFire = true;
	if (GunInfo.FireModes[CurrentFireMode] != EFireMode::Single)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunInfo.Sound_Loop_TailClose, SkeletalMeshComponent->GetSocketLocation(FirePosName));
	}
}

void AGun::SetScope(TObjectPtr<AScope> NewScope)
{
	if (Scope != NULL && Scope->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, "[SetScope] Scope is already exist");
		return;
	}
	Scope = NewScope;
	Scope->AttachToGun(SkeletalMeshComponent);
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

	//// Todo : [현재 발사 모드 화면]출력 기능 구현 필요
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, UEnum::GetValueAsString(GunInfo.FireModes[CurrentFireMode]));
}