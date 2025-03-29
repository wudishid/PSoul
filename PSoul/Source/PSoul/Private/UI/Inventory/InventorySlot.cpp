// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/KismetTextLibrary.h"

void UInventorySlot::UpdateSlot()
{
	if (UInventoryManagerComponent* InventoryManagerComponent = GetOwningPlayerPawn()->FindComponentByClass<
		UInventoryManagerComponent>())
	{
		FInventoryItemSlot ItemSlot;
		if (InventoryManagerComponent->GetItemSlotByIndex(SlotIndex, ItemSlot))
		{
			if (ItemSlot.IsValidSlot())
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

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		FVector2d MousePosition;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), MousePosition.X, MousePosition.Y);
		OnSlotRightMouseButtonDown.Broadcast(SlotIndex, MousePosition);
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
