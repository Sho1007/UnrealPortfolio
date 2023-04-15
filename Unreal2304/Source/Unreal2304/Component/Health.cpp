// Fill out your copyright notice in the Description page of Project Settings.


#include "../Component/Health.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"
#include "../Object/Item/Bullet.h"

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
	// Todo : Status 를 Data에서 받아서 Init해주기
	StatusArray.Init(false, (uint8)EStatusType::Size);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, FString::Printf(TEXT("StatusArray Size : %d"), StatusArray.Num()));


}

void UHealth::BlackOut()
{
	if (bIsBlackOut) return;
	bIsBlackOut = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("BlackOuted")));
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
	if (TObjectPtr<ABullet> Bullet = Cast<ABullet>(OtherActor))
	{
		OtherActor->Destroy();
		ApplyDamage(Bullet->GetDamage(), EDamageType::Physics);

		if (FMath::Rand() % 101 < Bullet->GetLightBleedChance())
		{
			LightBleed();
		}
		if (FMath::Rand() % 101 < Bullet->GetLightBleedChance())
		{
			HeavyBleed();
		}
	}
}

void UHealth::LightBleed()
{
	if (StatusArray[(uint8)EStatusType::LightBleeding]) return;
	StatusArray[(uint8)EStatusType::LightBleeding] = true;
	Owner->AddLightBleed();
	// Bleed Image 추가
}

void UHealth::HeavyBleed()
{
	if (StatusArray[(uint8)EStatusType::HeavyBleeding]) return;
	StatusArray[(uint8)EStatusType::HeavyBleeding] = true;
	Owner->AddHeavyBleed();
	// Heavy Bleed Image 추가
}

void UHealth::Disactive()
{
	for (int i = 0; i < CapsuleArray.Num(); ++i)
	{
		CapsuleArray[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UHealth::ApplyDamage(float NewDamage, EDamageType NewDamageType)
{
	if (bIsBlackOut)
	{
		// 현재 블랙아웃 상태
		switch (BodyType)
		{
		case EBodyType::Head:
		case EBodyType::Thorax:
			Owner->Dead();
			break;
		case EBodyType::Stomach:
			Owner->DesperseDamage(NewDamage, 1.5f);
			break;
		case EBodyType::RightArm:
			Owner->DesperseDamage(NewDamage, 0.7f);
			break;
		case EBodyType::LeftArm:
			Owner->DesperseDamage(NewDamage, 0.7f);
			break;
		case EBodyType::RightLeg:
			Owner->DesperseDamage(NewDamage, 1.0f);
			break;
		case EBodyType::LeftLeg:
			Owner->DesperseDamage(NewDamage, 1.0f);
			break;
		}
	}
	else
	{
		// 블랙 아웃이 아닌 상태
		HealthCurrent = HealthCurrent - NewDamage;
		if (HealthCurrent <= 0)
		{
			HealthCurrent = 0;
			BlackOut();
			if ((BodyType == EBodyType::Thorax || BodyType == EBodyType::Head ) && NewDamageType != EDamageType::Bleed)
			{
				Owner->Dead();
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s : %f"), *Name.ToString(), HealthCurrent);
	//GEngine->AddOnScreenDebugMessage(-1, 300, FColor::Yellow, FString::Printf(TEXT("%s Health : %f"), *Name.ToString(), HealthCurrent));
}