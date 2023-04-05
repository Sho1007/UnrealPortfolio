// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Bullet.h"

ABullet::ABullet()
{
	//PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// Todo : Hit 할때 호출함 함수 만들고 추가
	// SphereComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	SetRootComponent(SphereComponent);


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StataicMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
}

void ABullet::Fire(FTransform FireTransform)
{
	SetActorLocation(FireTransform.GetLocation());
	SetActorRotation(FireTransform.GetRotation());	
	ProjectileMovementComponent->SetVelocityInLocalSpace(GetActorForwardVector() * Speed);
}
