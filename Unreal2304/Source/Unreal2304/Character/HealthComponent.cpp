// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "../Object/Item/Bullet.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::CreateCapsule(float NewHalfHeight, float NewRadius, FName NewSocketName, FTransform NewRelativeTransform)
{
	//UE_LOG(LogTemp, Warning, TEXT("HealthComponent Owner is %s"), *GetOwner()->GetName());
	if (GetOwner() == NULL || !GetOwner()->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Owner is Not Valid");
		return;
	}

	UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(GetOwner()->AddComponentByClass(UCapsuleComponent::StaticClass(), true, FTransform::Identity, false));
	//TObjectPtr<UCapsuleComponent> CapsuleComponent = NewObject<UCapsuleComponent>();
	//
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetCollisionProfileName("OverlapAllDynamic");
	//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CapsuleComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleComponent->SetCapsuleSize(NewRadius, NewHalfHeight);
	FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	CapsuleComponent->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), AttachRule, NewSocketName);
	CapsuleComponent->SetRelativeTransform(NewRelativeTransform);
	CapsuleArray.Add(CapsuleComponent);
	CapsuleComponent->SetVisibility(true);
	CapsuleComponent->SetHiddenInGame(false);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &UHealthComponent::CapusleBeginOverlap);
	if (CapsuleComponent == NULL || !CapsuleComponent->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("Comp is NULL"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Comp Name : %s"), *CapsuleComponent->GetName());
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::CapusleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABullet>(OtherActor) == NULL) return;
	for (uint8 i = 0; i < CapsuleArray.Num(); ++i)
	{
		if (CapsuleArray[i] != OverlappedComponent) continue;

		if (i + 1 >= (uint8)EBodyType::Size)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "[CapusleBeginOverlap] Invalid i");
		}
		else
		{
			switch (static_cast<EBodyType>(i + 1))
			{
			case EBodyType::Head:
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "Head");
				break;
			case EBodyType::Thorax:
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "Thorax");
				break;
			case EBodyType::Stomach:
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "Stomach");
				break;
			case EBodyType::RightUpperArm:
			case EBodyType::RightLowerArm:
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "RightArm");
				// Right Arm
				break;
			case EBodyType::LeftUpperArm:
			case EBodyType::LeftLowerArm:
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "LeftArm");
				// Left Arm
				break;
			case EBodyType::RightThigh:
			case EBodyType::RightCalf:
			case EBodyType::RightBall:
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "RightLeg");
				// Right Leg
				break;
			case EBodyType::LeftThigh:
			case EBodyType::LeftCalf:
			case EBodyType::LeftBall:
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "LeftLeg");
				// Left Leg
				break;
			}
		}

		break;
	}
	
	// Todo : 그냥 사라지지 않고 Frection(파편화) 작업
	OtherActor->Destroy();
}
