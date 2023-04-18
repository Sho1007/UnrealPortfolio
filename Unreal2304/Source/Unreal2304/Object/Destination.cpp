// Fill out your copyright notice in the Description page of Project Settings.


#include "../Object/Destination.h"
#include "Components/BoxComponent.h"
#include "../Character/MyPlayer.h"

// Sets default values
ADestination::ADestination()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetMobility(EComponentMobility::Static);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName("DestinationBox");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestination::OnComponentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADestination::OnComponentEndOverlap);
}

// Called when the game starts or when spawned
void ADestination::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestination::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "OnComponentBeginOverlap Called");
	if (TObjectPtr<AMyPlayer> Player = Cast<AMyPlayer>(OtherActor))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "Player Begin Overlaped with Destination");
		
	}
}

void ADestination::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TObjectPtr<AMyPlayer> Player = Cast<AMyPlayer>(OtherActor))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "Player End Overlaped with Destination");
	}
}

