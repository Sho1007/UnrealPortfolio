// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widget/MenuBoxWidget.h"
#include "../Widget/MenuBoxChildWidget.h"

void UMenuBoxWidget::Initialize(TArray<FText>& MenuText)
{
	for (FText& Text : MenuText)
	{
		TObjectPtr<UMenuBoxChildWidget> Child = Cast<UMenuBoxChildWidget>(CreateWidget(GetWorld(), MenuBoxChildWidgetClass));
		Child->SetText(Text);
		VB_MenuBox->AddChildToVerticalBox(Child);
	}
	ToggleChildAt(0);
}

void UMenuBoxWidget::ToggleChildAt(int32 ChildPos)
{
	Cast<UMenuBoxChildWidget>(VB_MenuBox->GetChildAt(ChildPos))->Toggle();
}

void UMenuBoxWidget::Toggle(bool bIsDown)
{
	if (bIsDown && CurrentPos == VB_MenuBox->GetChildrenCount() - 1) return;
	else if (!bIsDown && CurrentPos == 0) return;

	Cast<UMenuBoxChildWidget>(VB_MenuBox->GetChildAt(CurrentPos))->Toggle();

	CurrentPos = bIsDown ? CurrentPos + 1 : CurrentPos - 1;

	Cast<UMenuBoxChildWidget>(VB_MenuBox->GetChildAt(CurrentPos))->Toggle();
}
