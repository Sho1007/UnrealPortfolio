// Fill out your copyright notice in the Description page of Project Settings.

#include "../Item/Item.h"

void AItem::Interact(TObjectPtr<AMyCharacter> Character)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString("Item"));
}

TArray<FText>& AItem::GetMenuText()
{
	return MenuText;
}
