// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/MyCharacter.h"
#include "Math/RotationMatrix.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Interface/Interactive.h"
#include "../Object/Item/Scope.h"
#include "../Component/HealthComponent.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh_Object(TEXT("'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (SK_Mesh_Object.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mesh_Object.Object);
	}
	

	// Camera Component Settings

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Health Component;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay(); 
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	SpringArmComponent->AttachToComponent(GetMesh(), AttachRules, "head");
	CameraComponent->SetRelativeLocation(FVector(10, 0, 0));
	//SpringArmComponent->ResetRelativeTransform();
	GunRecoilFactor = GunRecoilFactor_Base;

	// Health Component
	
	/*
	HealthComponent->CreateHealth("Head", 50, 50);
	HealthComponent->CreateHealth("Thorax", 50, 50);
	HealthComponent->CreateHealth("Stomach", 50, 50);
	HealthComponent->CreateHealth("RightArm", 50, 50);
	HealthComponent->CreateHealth("LeftArm", 50, 50);
	HealthComponent->CreateHealth("RightLeg", 50, 50);
	HealthComponent->CreateHealth("LeftLeg", 50, 50);
	*/
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	
}

bool AMyCharacter::EquipItem(TObjectPtr<AEquipment> Item)
{
	switch (Item->GetEquipmentType())
	{
	case EEquipmentType::None:
		break;
	case EEquipmentType::Gun:
		if (EquipGun(Cast<AGun>(Item))) return true;
		break;
	case EEquipmentType::HeadGear:
		break;
	case EEquipmentType::HeadSet:
		break;
	case EEquipmentType::Face:
		break;
	case EEquipmentType::Rig:
		break;
	case EEquipmentType::Armor:
		break;
	case EEquipmentType::ArmorRig:
		break;
	case EEquipmentType::Size:
		break;
	default:
		break;
	}
	return false;
}

bool AMyCharacter::PickupItem(FItemData ItemData)
{
	return false;
}

bool AMyCharacter::EquipGun(TObjectPtr<AGun> GunActor)
{
	if (PrimaryGun == NULL || !PrimaryGun->IsValidLowLevelFast())
	{
		PrimaryGun = GunActor;
		UpdateGunAttachment();
		return true;
	}
	else if (SecondaryGun == NULL || !SecondaryGun->IsValidLowLevelFast())
	{
		SecondaryGun = GunActor;
		UpdateGunAttachment();
		return true;
	}

	return false;
}

bool AMyCharacter::EquipScope(TObjectPtr<AScope> NewScope)
{
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		EquippedGun()->SetScope(NewScope);
		return true;
	}

	return false;
}

FVector AMyCharacter::GetZeroPointLocation()
{
	FVector ZeroPointLocation = CameraComponent->GetComponentLocation() + (CameraComponent->GetForwardVector() * ZeroPoint * 100);
	return ZeroPointLocation;
}

void AMyCharacter::ApplyGunRecoil(int32 VerticalRecoil, int32 HorizontalRecoil)
{
	// Todo : Recoil 적용하는 공식이 확인 후 재적용
	AddControllerPitchInput(VerticalRecoil * GunRecoilFactor);
	AddControllerYawInput(HorizontalRecoil * GunRecoilFactor);
}

void AMyCharacter::PlayGunFireAnimMontage(float Rate)
{
	if (bAimButtonDown)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(FireIronsight_AnimMontage);
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_Play(FireHip_AnimMontage);
	}
}

void AMyCharacter::UpdateGunAttachment()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateGunAttachment Called"));
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		EquippedGun()->AttachToCharacter(this, "GunHoldSocket");
	}
	if (StowedGun() != NULL && StowedGun()->IsValidLowLevelFast())
	{
		StowedGun()->AttachToCharacter(this, "GunStowedSocket");
	}
}

TObjectPtr<AGun> AMyCharacter::EquippedGun()
{
	return CurrentGunSlot == EGunSlot::Primary ? PrimaryGun : SecondaryGun;
}

TObjectPtr<AGun> AMyCharacter::StowedGun()
{
	return CurrentGunSlot == EGunSlot::Primary ? SecondaryGun : PrimaryGun;
}

void AMyCharacter::AdjustGunRecoilFactor()
{
	if (GunRecoilFactor > GunRecoilFactor_Min)
	{
		GunRecoilFactor -= GunRecoilFactor_Adjust_Amount;
	}

	AddControllerPitchInput(GunRecoilFactor_Base - GunRecoilFactor);
}

void AMyCharacter::Interact(TObjectPtr<AActor> Actor, uint8 SelectNum)
{
	ECharacterInteract InteractType = (ECharacterInteract)SelectNum;

	switch (InteractType)
	{
		// EquipItem 실패 시, PickupItem 실행 흐름
	case ECharacterInteract::PickupEquipment:
		if (EquipItem(Cast<AEquipment>(Actor))) break;
	case ECharacterInteract::PickupItem:
		break;
	}
}

TObjectPtr<TArray<FText>> AMyCharacter::GetMenuText()
{
	if (!HealthComponent->CheckIsDead())
	{
		return NULL;
	}
	return &MenuText;
}

