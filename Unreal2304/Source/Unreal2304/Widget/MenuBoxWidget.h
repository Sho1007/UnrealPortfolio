// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBoxWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2304_API UMenuBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Initialize(TArray<FText>& MenuText);
};
