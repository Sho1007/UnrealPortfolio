// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widget/MenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_OverAll->OnClicked.AddDynamic(this, &UMenuWidget::OnOverAllButtonClicked);
	Btn_Gear->OnClicked.AddDynamic(this, &UMenuWidget::OnGearButtonClicked);
	Btn_Health->OnClicked.AddDynamic(this, &UMenuWidget::OnHealthButtonClicked);
	Btn_Skills->OnClicked.AddDynamic(this, &UMenuWidget::OnSkillsButtonClicked);
	Btn_Map->OnClicked.AddDynamic(this, &UMenuWidget::OnMapButtonClicked);
	Btn_Tasks->OnClicked.AddDynamic(this, &UMenuWidget::OnTasksButtonClicked);
}

void UMenuWidget::OnOverAllButtonClicked()
{
	int32 TargetIndex = static_cast<int32>(EMenuType::OverAll);
	// Todo Here : Play Button Click Sound
	if (WidgetSwitcher->GetActiveWidgetIndex() == TargetIndex) return;
	Btn_OverAll->BackgroundColor = ButtonClickedColor;
	WidgetSwitcher->SetActiveWidgetIndex(TargetIndex);
}

void UMenuWidget::OnGearButtonClicked()
{
	int32 TargetIndex = static_cast<int32>(EMenuType::Gear);
	// Todo Here : Play Button Click Sound
	if (WidgetSwitcher->GetActiveWidgetIndex() == TargetIndex) return;
	WidgetSwitcher->SetActiveWidgetIndex(TargetIndex);
}

void UMenuWidget::OnHealthButtonClicked()
{
	int32 TargetIndex = static_cast<int32>(EMenuType::Health);
	// Todo Here : Play Button Click Sound
	if (WidgetSwitcher->GetActiveWidgetIndex() == TargetIndex) return;
	WidgetSwitcher->SetActiveWidgetIndex(TargetIndex);
}

void UMenuWidget::OnSkillsButtonClicked()
{
	int32 TargetIndex = static_cast<int32>(EMenuType::Skills);
	// Todo Here : Play Button Click Sound
	if (WidgetSwitcher->GetActiveWidgetIndex() == TargetIndex) return;
	WidgetSwitcher->SetActiveWidgetIndex(TargetIndex);
}

void UMenuWidget::OnMapButtonClicked()
{
	int32 TargetIndex = static_cast<int32>(EMenuType::Map);
	// Todo Here : Play Button Click Sound
	if (WidgetSwitcher->GetActiveWidgetIndex() == TargetIndex) return;
	WidgetSwitcher->SetActiveWidgetIndex(TargetIndex);
}

void UMenuWidget::OnTasksButtonClicked()
{
	int32 TargetIndex = static_cast<int32>(EMenuType::Tasks);
	// Todo Here : Play Button Click Sound
	if (WidgetSwitcher->GetActiveWidgetIndex() == TargetIndex) return;
	WidgetSwitcher->SetActiveWidgetIndex(TargetIndex);
}