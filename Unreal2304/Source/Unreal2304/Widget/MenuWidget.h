// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMenuType : uint8
{
	OverAll,
	Gear,
	Health,
	Skills,
	Map,
	Tasks,
	Size
};

class UButton;
class UWidgetSwitcher;
UCLASS()
class UNREAL2304_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnOverAllButtonClicked();
	UFUNCTION()
	void OnGearButtonClicked();
	UFUNCTION()
	void OnHealthButtonClicked();
	UFUNCTION()
	void OnSkillsButtonClicked();
	UFUNCTION()
	void OnMapButtonClicked();
	UFUNCTION()
	void OnTasksButtonClicked();

private:
	TArray<TObjectPtr<UButton>> ButtonArray;



	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> Btn_OverAll;
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> Btn_Gear;
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> Btn_Health;
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> Btn_Skills;
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> Btn_Map;
	UPROPERTY(meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> Btn_Tasks;
};
