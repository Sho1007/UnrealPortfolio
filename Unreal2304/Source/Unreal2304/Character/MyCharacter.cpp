// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/MyCharacter.h"
#include "Math/RotationMatrix.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Interface/Interactive.h"
#include "../Widget/MenuBoxWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Camera Component Settings

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Menu Box Widget Component Settings
	MenuBoxWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("MenuBoxWidgetComponent");
	MenuBoxWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	MenuBoxWidgetComponent->SetDrawAtDesiredSize(true);
	MenuBoxWidgetComponent->SetupAttachment(CameraComponent);
	MenuBoxWidgetComponent->SetRelativeLocation(FVector(1, 0, 0));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	SpringArmComponent->AttachToComponent(GetMesh(), AttachRules, "head");
	CameraComponent->SetRelativeLocation(FVector(10, 0, 0));
	//SpringArmComponent->ResetRelativeTransform();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckInteract();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AMyCharacter::TurnRight);

	PlayerInputComponent->BindAction("Wheel Up", EInputEvent::IE_Pressed, this, &AMyCharacter::WheelUp);
	PlayerInputComponent->BindAction("Wheel Down", EInputEvent::IE_Pressed, this, &AMyCharacter::WheelDown);

	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AMyCharacter::Crouch);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMyCharacter::JumpPressed);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AMyCharacter::JumpReleased);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AMyCharacter::InteractPressed);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AMyCharacter::AttackPressed);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &AMyCharacter::AttackReleased);

	PlayerInputComponent->BindAction("ChangeFireMode", EInputEvent::IE_Pressed, this, &AMyCharacter::ChangeFireMode);
}

void AMyCharacter::CheckInteract()
{

	FVector StartPoint = GetMesh()->GetSocketLocation("head");
	FVector EndPoint = StartPoint + (CameraComponent->GetForwardVector() * InteractCheckLength);
	FHitResult OutHit;
	TArray<TEnumAsByte<ETraceTypeQuery>> QueryArray;
	QueryArray.Add(UEngineTypes::ConvertToTraceType(ECC_Visibility));
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPoint, EndPoint, InteractCheckRadius, ETraceTypeQuery::TraceTypeQuery1, false, {}, EDrawDebugTrace::ForOneFrame, OutHit, true))
	{
		if (TObjectPtr<IInteractive> Interface = Cast<IInteractive>(OutHit.GetActor()))
		{
			// Check Already Has Old Actor
			if (InteractActor != NULL && InteractActor->IsValidLowLevelFast())
			{
				// IsSame Old Actor, New Actor
				if (InteractActor == OutHit.GetActor())
				{
					return;
				}
				else
				{
					DeleteMenuBoxWidget();
					// Todo : Detach Old Actor
				}
			}
			InteractActor = OutHit.GetActor();
			CreateMenuBoxWidget(Interface->GetMenuText());

			return;
		}
	}

	if (InteractActor != NULL && InteractActor->IsValidLowLevelFast())
	{
		// Todo : Detach Old InteractActor
		DeleteMenuBoxWidget();
		InteractActor = NULL;
	}
}

void AMyCharacter::CreateMenuBoxWidget(TArray<FText>& _MenuText)
{
	if (MenuBoxWidgetClass->IsValidLowLevelFast())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "4. MenuBox Widget Class is Valid");
		MenuBoxWidget = Cast<UMenuBoxWidget>(CreateWidget(GetWorld(), MenuBoxWidgetClass));
		MenuBoxWidget->Initialize(_MenuText);
		MenuBoxWidgetComponent->SetWidget(MenuBoxWidget);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "5. MenuBox Widget Class is Not Valid");
	}
}

void AMyCharacter::DeleteMenuBoxWidget()
{
	if (MenuBoxWidget->IsValidLowLevelFast())
	{
		MenuBoxWidget->RemoveFromRoot();
	}
	MenuBoxWidget = NULL;
	MenuBoxWidgetComponent->SetWidget(NULL);
}

void AMyCharacter::MoveForward(float Value)
{
	FVector MoveDirection = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(MoveDirection, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	FVector MoveDirection = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(MoveDirection, Value);
}

void AMyCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyCharacter::TurnRight(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter::WheelUp()
{
	if (MenuBoxWidget != NULL && MenuBoxWidget->IsValidLowLevelFast())
	{
		MenuBoxWidget->Toggle(false);
	}
}

void AMyCharacter::WheelDown()
{
	if (MenuBoxWidget != NULL && MenuBoxWidget->IsValidLowLevelFast())
	{
		MenuBoxWidget->Toggle(true);
	}
}

void AMyCharacter::Crouch()
{
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = bCrouchButtonDown ? 270 : 100;

	bCrouchButtonDown = !bCrouchButtonDown;
}

void AMyCharacter::JumpPressed()
{
	Jump();
	bJumpButtonDown = CanJump();
	TObjectPtr<UCharacterMovementComponent> CharacterMovementCompoenent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterMovementCompoenent == NULL || !CharacterMovementCompoenent->IsValidLowLevelFast()) return;
	FVector Velocity = CharacterMovementCompoenent->Velocity;
	CharacterMovementCompoenent->JumpZVelocity = Velocity.Length() > 0.0f ? 400 : 200;
}

void AMyCharacter::JumpReleased()
{
	StopJumping();
	bJumpButtonDown = false;
}

void AMyCharacter::InteractPressed()
{
	if (InteractActor == NULL || !InteractActor->IsValidLowLevelFast()) return;

	Cast<IInteractive>(InteractActor)->Interact(this, MenuBoxWidget->GetSelectNum());
}

void AMyCharacter::AttackPressed()
{
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		EquippedGun()->Fire();
	}
}

void AMyCharacter::AttackReleased()
{
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		EquippedGun()->FireStop();
	}
}

void AMyCharacter::ChangeFireMode()
{
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		EquippedGun()->ChangeFireMode();
	}
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
		GunActor->SetState(EItemState::Equipped);
		UpdateGunAttachment();
		return true;
	}
	else if (SecondaryGun == NULL || !SecondaryGun->IsValidLowLevelFast())
	{
		SecondaryGun = GunActor;
		GunActor->SetState(EItemState::Equipped);
		UpdateGunAttachment();
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

TArray<FText>& AMyCharacter::GetMenuText()
{
	return MenuText;
}

