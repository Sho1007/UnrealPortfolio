// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widget/MenuBoxChildWidget.h"

void UMenuBoxChildWidget::Toggle()
{
	if (Border->GetBrushColor() == ActiveColor)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, "0");
		Border->SetBrushColor(DisactiveColor);
		Text->SetColorAndOpacity(FSlateColor(FColor::White));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, "1");
		Border->SetBrushColor(ActiveColor);
		Text->SetColorAndOpacity(FSlateColor(FColor::Black));
	}
}
