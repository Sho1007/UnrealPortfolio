// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Bullet.h"

ABullet::ABullet()
{
	//PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	//CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->BodyInstance.SetCollisionProfileName("OverlapAll");
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnComponentBeginOverlap);
	CollisionComponent->SetSphereRadius(0.5f);
	SetRootComponent(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ABullet::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor == this) return;
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, OtherActor->GetName());
}

void ABullet::Fire(FVector MoveDirection)
{
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->Velocity = MoveDirection * Speed;
}