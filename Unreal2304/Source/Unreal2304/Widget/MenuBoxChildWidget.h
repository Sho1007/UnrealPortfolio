// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "MenuBoxChildWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2304_API UMenuBoxChildWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> Text;
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UBorder> Border;
};
