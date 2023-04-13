// Fill out your copyright notice in the Description page of Project Settings.


#include "../Component/Health.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"

void UHealth::Init(TObjectPtr<UHealthComponent> NewOwner, FName NewName, EBodyType NewBodyType, float NewHealthCurrent, float NewHealthMax)
{
	Owner = NewOwner;
	Name = NewName;
	BodyType = NewBodyType;
	HealthCurrent = NewHealthCurrent;
	HealthMax = NewHealthMax;
	if (HealthCurrent == 0)
	{
		BlackOut();
	}
	StatusArray.Init(false, (uint8)EStatusType::Size);
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, FString::Printf(TEXT("StatusArray Size : %d"), StatusArray.Num()));
}

void UHealth::BlackOut()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("%s BlackOuted"), *Name.ToString()));
}

void UHealth::AddCapsule(TObjectPtr<USkeletalMeshComponent> NewMesh, float NewHalfHeight, float NewRadius, FName NewSocketName, FTransform NewRelativeTransform)
{
	TObjectPtr<UCapsuleComponent> NewCapsule = Cast<UCapsuleComponent>(NewMesh->GetOwner()->AddComponentByClass(UCapsuleComponent::StaticClass(), true, FTransform::Identity, false));
	NewCapsule->SetGenerateOverlapEvents(true);
	NewCapsule->SetCollisionProfileName("HealthCapsule");
	NewCapsule->SetCapsuleSize(NewRadius, NewHalfHeight);
	FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	NewCapsule->AttachToComponent(NewMesh, AttachRule, NewSocketName);
	NewCapsule->SetRelativeTransform(NewRelativeTransform);
	NewCapsule->SetVisibility(true);
	NewCapsule->SetHiddenInGame(false);
	NewCapsule->OnComponentBeginOverlap.AddDynamic(this, &UHealth::OnComponentBeginOverlap);
	CapsuleArray.Add(NewCapsule);
}

void UHealth::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyDamage(10.0f, EDamageType::Physics);
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("%s Health : %f"), *Name.ToString(), HealthCurrent));
}

void UHealth::Bleed()
{
	if (bIsBleed) return; bIsBleed = true;
	GetWorld()->GetTimerManager().SetTimer(BleedTimerHandle, this, &UHealth::BleedFunction, 6, true);
	// Bleed Image 추가
}
void UHealth::BleedFunction()
{

}

void UHealth::ApplyDamage(float NewDamage, EDamageType NewDamageType)
{
	if (HealthCurrent == 0)
	{
		// 현재 블랙아웃 상태
	}
	else
	{
		// 블랙 아웃이 아닌 상태
		HealthCurrent = FMath::Max(0, HealthCurrent - NewDamage);
		if (HealthCurrent == 0)
		{
			if (BodyType == EBodyType::Thorax || (BodyType == EBodyType::Head && NewDamageType != EDamageType::Bleed))
			{
				// Todo : Character Death
			}
			BlackOut();
		}
	}
}
