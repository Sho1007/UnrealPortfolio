// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
class UWidgetSwitcher;
UCLASS()
class UNREAL2304_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SwitchWidget();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
private:
	// Widget Switcher Var
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
};
