// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemInfoWidget.h"

#include "Components/TextBlock.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Kismet/KismetTextLibrary.h"

void UItemInfoWidget::UpdateItemInfo(const FInventoryItemInfo& InItemInfo)
{
	Text_ItemName->SetText(UKismetTextLibrary::Conv_NameToText(InItemInfo.ItemName));
	Text_ItemDesc->SetText(UKismetTextLibrary::Conv_StringToText(InItemInfo.Description));
}

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
