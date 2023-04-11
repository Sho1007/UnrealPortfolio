// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/Equipment.h"

#include "Scope.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2304_API AScope : public AEquipment
{
	GENERATED_BODY()
public:
	AScope();
private:
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = NULL;
	TObjectPtr<class USceneCaptureComponent2D> SceneCaptureComponent2D = NULL;
};
