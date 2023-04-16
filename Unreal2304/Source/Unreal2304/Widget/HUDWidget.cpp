// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widget/HUDWidget.h"
#include "Components/WidgetSwitcher.h"
#include "../Character/MyPlayer.h"

void UHUDWidget::SwitchWidget()
{
	
	if (WidgetSwitcher->GetActiveWidgetIndex() == 0)
	{
		// ���� InGameWidget �� ���
		WidgetSwitcher->SetActiveWidgetIndex(1);
		FInputModeUIOnly NewInputMode;
		NewInputMode.SetWidgetToFocus(this->TakeWidget());
		GetOwningPlayer()->SetInputMode(NewInputMode);
		GetOwningPlayer()->SetShowMouseCursor(true);
	}
	else
	{
		// ���� GearWidget �� ���
		WidgetSwitcher->SetActiveWidgetIndex(0);
		FInputModeGameOnly NewInputMode;
		GetOwningPlayer()->SetInputMode(NewInputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
}

FReply UHUDWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "Tab Pressed");
		SwitchWidget();
	}

	return UUserWidget::NativeOnKeyDown(InGeometry, InKeyEvent);
}