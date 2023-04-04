// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Bullet.h"

ABullet::ABullet()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StataicMeshComponent");
	SetRootComponent(StaticMeshComponent);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
}

void ABullet::Fire(FVector StartPos, FVector Direction)
{
	SetActorLocation(StartPos);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Direction.Normalize();
	ProjectileMovementComponent->SetVelocityInLocalSpace(Direction * Speed);
}
