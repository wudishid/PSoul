// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Kismet/KismetTextLibrary.h"

void UInventorySlot::UpdateSlot(FInventoryItemSlot& ItemSlot)
{
	if(ItemSlot.IsValidSlot())
	{
		Image_Icon->SetBrushFromTexture(ItemSlot.ItemInfo.Icon.LoadSynchronous());
		Image_Icon->SetVisibility(ESlateVisibility::Visible);
		Text_Amount->SetText(UKismetTextLibrary::Conv_IntToText(ItemSlot.Amount));
		Text_Amount->SetVisibility(ESlateVisibility::Visible);
		bEmpty = false;
	}
	else
	{
		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
		Text_Amount->SetVisibility(ESlateVisibility::Collapsed);
		bEmpty = true;
	}
}

bool UInventorySlot::IsEmpty() const
{
	return bEmpty;
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}
