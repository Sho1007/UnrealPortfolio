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

	void BeginPlay() override;

public:
	void AttachToGun(TObjectPtr<USkeletalMeshComponent> TargaetSkeletalMeshComponent);
	virtual void Interact(TObjectPtr<AActor> Character, uint8 SelectNum) override;
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = NULL;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ScopeMesh = NULL;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USceneCaptureComponent2D> SceneCaptureComponent2D = NULL;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMaterialInterface> ScopeMaterialInterface = NULL;
	TObjectPtr<class UTextureRenderTarget2D> RenderTarget;
};
