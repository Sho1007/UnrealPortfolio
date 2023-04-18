// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/MyPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Widget/MenuBoxWidget.h"
#include "../Widget/HUDWidget.h"

AMyPlayer::AMyPlayer()
	: AMyCharacter()
{
	// Menu Box Widget Component Settings
	MenuBoxWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("MenuBoxWidgetComponent");
	MenuBoxWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	MenuBoxWidgetComponent->SetDrawAtDesiredSize(true);
	MenuBoxWidgetComponent->SetupAttachment(CameraComponent);
	MenuBoxWidgetComponent->SetRelativeLocation(FVector(1, 0, 0));

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBoxWidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widgets/WBP_MenuBox.WBP_MenuBox_C'"));

	if (MenuBoxWidgetClassFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AMyPlayer] Find MenuBox Widget Class Successfully"));
		MenuBoxWidgetClass = MenuBoxWidgetClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AMyPlayer] MenuBox Widget Class is Not Valid"));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/WBP_HUD.WBP_HUD_C'"));

	if (HUDWidgetClassFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AMyPlayer] Find HUD Widget Class Successfully"));
		HUDWidgetClass = HUDWidgetClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AMyPlayer] HUD Widget Class is Not Valid"));
	}
}

void AMyPlayer::BeginPlay()
{
	AMyCharacter::BeginPlay();
	HUDWidget = CreateWidget<UHUDWidget>(Cast<APlayerController>(GetController()), HUDWidgetClass);
	HUDWidget->AddToPlayerScreen(0);
}

void AMyPlayer::Tick(float DeltaTime)
{
	AMyCharacter::Tick(DeltaTime);
	CheckInteract();
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AMyPlayer::LookUp);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AMyPlayer::TurnRight);

	PlayerInputComponent->BindAction("Wheel Up", EInputEvent::IE_Pressed, this, &AMyPlayer::WheelUp);
	PlayerInputComponent->BindAction("Wheel Down", EInputEvent::IE_Pressed, this, &AMyPlayer::WheelDown);

	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AMyPlayer::CrouchPressed);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMyPlayer::JumpPressed);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AMyPlayer::JumpReleased);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AMyPlayer::InteractPressed);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AMyPlayer::AttackPressed);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &AMyPlayer::AttackReleased);

	PlayerInputComponent->BindAction("Aiming", EInputEvent::IE_Pressed, this, &AMyPlayer::AimingPressed);

	PlayerInputComponent->BindAction("StopBreath", EInputEvent::IE_Pressed, this, &AMyPlayer::StopBreathPressed);
	PlayerInputComponent->BindAction("StopBreath", EInputEvent::IE_Released, this, &AMyPlayer::StopBreathReleased);


	PlayerInputComponent->BindAction("ChangeFireMode", EInputEvent::IE_Pressed, this, &AMyPlayer::ChangeFireMode);

	PlayerInputComponent->BindAction("Tab", EInputEvent::IE_Pressed, this, &AMyPlayer::TabPressed);
}

void AMyPlayer::SetEscapeTimer(const float NewEscapeTime)
{
	EscapeWaitTime_Current = NewEscapeTime;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::Printf(TEXT("Escape Timer : %f"), EscapeWaitTime_Current));
	GetWorldTimerManager().SetTimer(EscapeTimerHandle, this, &AMyPlayer::EscapeTimer, 0.1f, true, 0.1f);
}

void AMyPlayer::CancleEscapeTimer()
{
	GetWorldTimerManager().ClearTimer(EscapeTimerHandle);
}

void AMyPlayer::EscapeTimer()
{
	EscapeWaitTime_Current = FMath::Max(EscapeWaitTime_Current - 0.1f, 0.0f);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::Printf(TEXT("Escape Timer : %f"), EscapeWaitTime_Current));
	if (EscapeWaitTime_Current <= 0.0f)
	{
		GetWorldTimerManager().ClearTimer(EscapeTimerHandle);
		Escape();
	}
}

void AMyPlayer::Escape()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::Printf(TEXT("Escaped!")));
}

void AMyPlayer::CheckInteract()
{

	FVector StartPoint = GetMesh()->GetSocketLocation("head");
	FVector EndPoint = StartPoint + (CameraComponent->GetForwardVector() * InteractCheckLength);
	FHitResult OutHit;
	TArray<TEnumAsByte<ETraceTypeQuery>> QueryArray;
	QueryArray.Add(UEngineTypes::ConvertToTraceType(ECC_Visibility));
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPoint, EndPoint, InteractCheckRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, {}, EDrawDebugTrace::ForOneFrame, OutHit, true))
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

void AMyPlayer::CreateMenuBoxWidget(TObjectPtr<TArray<FText>> _MenuText)
{
	if (_MenuText == NULL) return;
	if (MenuBoxWidgetClass->IsValidLowLevelFast())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "4. MenuBox Widget Class is Valid");
		MenuBoxWidget = Cast<UMenuBoxWidget>(CreateWidget(GetWorld(), MenuBoxWidgetClass));
		MenuBoxWidget->Init(*_MenuText);
		MenuBoxWidgetComponent->SetWidget(MenuBoxWidget);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "5. MenuBox Widget Class is Not Valid");
	}
}

void AMyPlayer::DeleteMenuBoxWidget()
{
	if (MenuBoxWidget->IsValidLowLevelFast())
	{
		MenuBoxWidget->RemoveFromRoot();
	}
	MenuBoxWidget = NULL;
	MenuBoxWidgetComponent->SetWidget(NULL);
}

void AMyPlayer::InteractPressed()
{
	if (InteractActor == NULL || !InteractActor->IsValidLowLevelFast()) return;

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("%s"), *InteractActor->GetName()));

	Cast<IInteractive>(InteractActor)->Interact(this, MenuBoxWidget->GetSelectNum());
}

void AMyPlayer::WheelUp()
{
	if (MenuBoxWidget != NULL && MenuBoxWidget->IsValidLowLevelFast())
	{
		MenuBoxWidget->Toggle(false);
	}
}

void AMyPlayer::WheelDown()
{
	if (MenuBoxWidget != NULL && MenuBoxWidget->IsValidLowLevelFast())
	{
		MenuBoxWidget->Toggle(true);
	}
}



void AMyPlayer::MoveForward(float Value)
{
	FVector MoveDirection = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(MoveDirection, Value);
}

void AMyPlayer::MoveRight(float Value)
{
	FVector MoveDirection = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(MoveDirection, Value);
}

void AMyPlayer::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyPlayer::TurnRight(float Value)
{
	AddControllerYawInput(Value);
}

void AMyPlayer::TabPressed()
{
	HUDWidget->SwitchWidget();
}

void AMyPlayer::CrouchPressed()
{
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = bCrouchButtonDown ? 270 : 100;

	bCrouchButtonDown = !bCrouchButtonDown;
}

void AMyPlayer::JumpPressed()
{
	Jump();
	bJumpButtonDown = CanJump();
	TObjectPtr<UCharacterMovementComponent> CharacterMovementCompoenent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterMovementCompoenent == NULL || !CharacterMovementCompoenent->IsValidLowLevelFast()) return;
	FVector Velocity = CharacterMovementCompoenent->Velocity;
	CharacterMovementCompoenent->JumpZVelocity = Velocity.Length() > 0.0f ? 400 : 200;
}

void AMyPlayer::JumpReleased()
{
	StopJumping();
	bJumpButtonDown = false;
}

void AMyPlayer::AttackPressed()
{
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		GetWorldTimerManager().SetTimer(GunRecoilTimerHandle, this, &AMyCharacter::AdjustGunRecoilFactor, GunRecoilFactor_Adjust_Rate, true);
		EquippedGun()->Fire();
	}
}

void AMyPlayer::AttackReleased()
{
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		EquippedGun()->FireStop();
		GetWorldTimerManager().ClearTimer(GunRecoilTimerHandle);
		GunRecoilFactor = GunRecoilFactor_Base;
	}
}

void AMyPlayer::AimingPressed()
{
	FAttachmentTransformRules AttachRule(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	if (bAimButtonDown)
	{
		bAimButtonDown = false;
		SpringArmComponent->AttachToComponent(GetMesh(), AttachRule, "head");
	}
	else
	{
		if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
		{
			bAimButtonDown = true;
			SpringArmComponent->AttachToComponent(EquippedGun()->GetGunMesh(), AttachRule, "ZoomFacePos");
		}
		else
		{
			bAimButtonDown = false;
			SpringArmComponent->AttachToComponent(GetMesh(), AttachRule, "head");
		}
	}
}

void AMyPlayer::StopBreathPressed()
{
	bStopBreath = true;
}

void AMyPlayer::StopBreathReleased()
{
	bStopBreath = false;
}

void AMyPlayer::ChangeFireMode()
{
	if (EquippedGun() != NULL && EquippedGun()->IsValidLowLevelFast())
	{
		EquippedGun()->ChangeFireMode();
	}
}