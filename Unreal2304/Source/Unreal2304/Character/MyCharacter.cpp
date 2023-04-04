// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/MyCharacter.h"
#include "Math/RotationMatrix.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Interface/Interactive.h"
#include "../Widget/MenuBoxWidget.h"

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
			Interface->Interact(this);

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

void AMyCharacter::CreateMenuBoxWidget(TArray<FText>& MenuText)
{
	if (MenuBoxWidgetClass->IsValidLowLevelFast())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "4. MenuBox Widget Class is Valid");
		MenuBoxWidget = Cast<UMenuBoxWidget>(CreateWidget(GetWorld(), MenuBoxWidgetClass));
		MenuBoxWidget->Initialize(MenuText);
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

