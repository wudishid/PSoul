// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Equipment/EquipmentManagerComponent.h"
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

EItemOpetaionType UInventorySlot::GetRulesForOperationType(EItemOpetaionType type)
{
	return type;
}

void UInventorySlot::HandleItemOperation(EItemOpetaionType OpetaionType)
{
	if(OpetaionType == EItemOpetaionType::Use)
	{
		
	}
	else if(OpetaionType == EItemOpetaionType::Drop)
	{
		InventoryManagerComp->DropItem(SlotIndex);
	}
	else if(OpetaionType == EItemOpetaionType::Equip)
	{
		FInventoryItemInfo ItemInfo;
		if(InventoryManagerComp->GetItemInfoByIndex(SlotIndex, ItemInfo))
		{
			EquipmentManagerComp->Equip(ItemInfo.EquipmentClass);
			InventoryManagerComp->RemoveItem(SlotIndex);
		}
	}
	else if(OpetaionType == EItemOpetaionType::UnEquip)
	{
		
	}
}

FInventoryItemInfo UInventorySlot::GetItemInfo() const
{
	FInventoryItemInfo ItemInfo;
	InventoryManagerComp->GetItemInfoByIndex(SlotIndex, ItemInfo);
	return ItemInfo;
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryManagerComp = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>();
	check(InventoryManagerComp);
	
	EquipmentManagerComp = GetOwningPlayerPawn()->FindComponentByClass<UEquipmentManagerComponent>();
	check(EquipmentManagerComp);
	
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		FVector2d MousePosition;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), MousePosition.X, MousePosition.Y);
		OnSlotRightMouseButtonDown.Broadcast(this, MousePosition);
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
