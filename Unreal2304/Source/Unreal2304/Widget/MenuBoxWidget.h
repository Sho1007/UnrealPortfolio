// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/VerticalBox.h"

#include "MenuBoxWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2304_API UMenuBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(TArray<FText>& MenuText);
	void ToggleChildAt(int32 ChildPos);
	void Toggle(bool bIsDown);
	uint8 GetSelectNum() { return CurrentPos; }
private:
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UVerticalBox> VB_MenuBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MenuBoxChildWidgetClass;
	uint8 CurrentPos = 0;
};
