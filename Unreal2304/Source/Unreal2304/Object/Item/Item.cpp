// Fill out your copyright notice in the Description page of Project Settings.

#include "../Item/Item.h"

AItem::AItem()
{
}

TArray<FText>& AItem::GetMenuText()
{
	return MenuText;
}
