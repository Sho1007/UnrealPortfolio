// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Scope.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "../../Character/MyCharacter.h"

AScope::AScope()
	: AEquipment(EEquipmentType::Scope)
{
	ItemData.ItemState = EItemState::Dropped;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName("DroppedItem");
	SetRootComponent(StaticMeshComponent);

	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScopeMesh");
	ScopeMesh->SetupAttachment(RootComponent);
	ScopeMesh->SetCollisionProfileName("NoCollision");

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCaptureComponent2D");
	SceneCaptureComponent2D->SetupAttachment(RootComponent);
}

void AScope::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitCustomFormat(2048, 2048, EPixelFormat::PF_FloatRGBA, false);
	SceneCaptureComponent2D->TextureTarget = RenderTarget;
	TObjectPtr<UMaterialInstanceDynamic> ScopeMaterial = UMaterialInstanceDynamic::Create(ScopeMaterialInterface, this);
	ScopeMaterial->SetTextureParameterValue("RenderTarget", RenderTarget);
	ScopeMesh->SetMaterial(0, ScopeMaterial);
}

void AScope::AttachToGun(TObjectPtr<USkeletalMeshComponent> TargaetSkeletalMeshComponent)
{
	ItemData.ItemState = EItemState::Equipped;
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetCollisionProfileName("NoCollision");
	FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	this->AttachToComponent(TargaetSkeletalMeshComponent, AttachRule, "ScopeSocket");
}

void AScope::Interact(TObjectPtr<AActor> Character, uint8 SelectNum)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, "[Interact] Function Called");

	Cast<AMyCharacter>(Character)->EquipScope(this);
}
