// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Development/Soul_UISetting.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/KismetTextLibrary.h"
#include "UI/Inventory/ItemInfoWidget.h"


void UInventorySlot::UpdateSlot()
{
	InventoryManagerComp = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>();
	check(InventoryManagerComp);

	EquipmentManagerComp = GetOwningPlayerPawn()->FindComponentByClass<UEquipmentManagerComponent>();
	check(EquipmentManagerComp);
	
	FInventoryItemSlot ItemSlot;
	if (InventoryManagerComp->GetItemSlotByIndex(SlotIndex, ItemSlot))
	{
		if (ItemSlot.IsValidSlot())
		{
			Image_Icon->SetBrushFromTexture(ItemSlot.ItemInfo.Icon.LoadSynchronous());
			Image_Icon->SetVisibility(ESlateVisibility::Visible);
			if (GetItemInfo().ItemType != EItemType::Equipment)
			{
				Text_Amount->SetText(UKismetTextLibrary::Conv_IntToText(ItemSlot.Amount));
				Text_Amount->SetVisibility(ESlateVisibility::Visible);
			}

			//提示控件
			if (UItemInfoWidget* InfoWidget = CreateWidget<UItemInfoWidget>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->ItemInfoClass.LoadSynchronous()))
			{
				InfoWidget->UpdateItemInfo(GetItemInfo());
				SetToolTip(InfoWidget);
			}
			
			bEmpty = false;
		}
		else
		{
			Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
			Text_Amount->SetVisibility(ESlateVisibility::Collapsed);

			//提示控件
			SetToolTip(nullptr);
			
			bEmpty = true;
		}
	}
	
}

bool UInventorySlot::IsEmpty() const
{
	return bEmpty;
}

EItemOperationType UInventorySlot::GetRulesForOperationType(EItemOperationType type)
{
	if(type == EItemOperationType::UnEquip)
	{
		return EItemOperationType::None;
	}
	return type;
}

void UInventorySlot::HandleItemOperation(EItemOperationType OpetaionType)
{
	if(OpetaionType == EItemOperationType::Use)
	{
		InventoryManagerComp->UseItem(SlotIndex);
	}
	else if(OpetaionType == EItemOperationType::Drop)
	{
		InventoryManagerComp->DropItem(SlotIndex);
	}
	else if(OpetaionType == EItemOperationType::Equip)
	{
		FInventoryItemInfo ItemInfo;
		if(InventoryManagerComp->GetItemInfoByIndex(SlotIndex, ItemInfo))
		{
			EquipmentManagerComp->Equip(ItemInfo.ItemName);
			InventoryManagerComp->RemoveItem(SlotIndex);
		}
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

