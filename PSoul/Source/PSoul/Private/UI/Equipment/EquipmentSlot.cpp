// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Equipment/EquipmentSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Equipment/EquipmentInstance.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"


EItemOpetaionType UEquipmentSlot::GetRulesForOperationType(EItemOpetaionType type)
{
	if(type == EItemOpetaionType::Equip)
	{
		return EItemOpetaionType::UnEquip;
	}
	return type;
}

void UEquipmentSlot::HandleItemOperation(EItemOpetaionType OpetaionType)
{
	if(OpetaionType == EItemOpetaionType::Drop)
	{
		EquipmentComponent->Drop(EquipmentType);
	}
	else if(OpetaionType == EItemOpetaionType::UnEquip)
	{
		InventoryManagerComp->AddItem(GetItemInfo());
		EquipmentComponent->UnEquip(GetItemInfo().EquipmentClass);
	}
}

FInventoryItemInfo UEquipmentSlot::GetItemInfo() const
{
	FInventoryItemInfo itemInfo;
	EquipmentComponent->GetWearedEquipmentInof(EquipmentType, itemInfo);
	return itemInfo;
}

void UEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryManagerComp = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>();
	check(InventoryManagerComp);
	
	EquipmentComponent = GetOwningPlayerPawn()->FindComponentByClass<UEquipmentManagerComponent>();
	check(EquipmentComponent);
	EquipmentComponent->OnEquip.AddUObject(this, &ThisClass::HandleEquip);
	EquipmentComponent->OnUnEquip.AddUObject(this, &ThisClass::HandleUnEquip);
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UEquipmentSlot::HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance)
{
	if (InEquipmentType == EquipmentType)
	{
		if (EquipmentInstance)
		{
			Image_Icon->SetVisibility(ESlateVisibility::Visible);
			Image_Icon->SetBrushFromTexture(EquipmentInstance->GetItemInfo().Icon.LoadSynchronous());
		}
	}
}

void UEquipmentSlot::HandleUnEquip(EEquipmentType InEquipmentType)
{
	if (InEquipmentType == EquipmentType)
	{
		Image_Icon->SetVisibility(ESlateVisibility::Hidden);
		Image_Icon->SetBrushFromTexture(nullptr);
	}
}

