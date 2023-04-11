// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Scope.h"
#include "Components/SceneCaptureComponent2D.h"

AScope::AScope()
	: AEquipment(EEquipmentType::Scope)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCaptureComponent2D");
	SceneCaptureComponent2D->SetupAttachment(RootComponent);
}