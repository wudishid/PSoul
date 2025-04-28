// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Equipment/EquipmentSlotWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Equipment/EquipmentInstance.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"


EItemOpetaionType UEquipmentSlotWidget::GetRulesForOperationType(EItemOpetaionType type)
{
	if(type == EItemOpetaionType::Equip)
	{
		return EItemOpetaionType::UnEquip;
	}
	return type;
}

void UEquipmentSlotWidget::HandleItemOperation(EItemOpetaionType OpetaionType)
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

FInventoryItemInfo UEquipmentSlotWidget::GetItemInfo() const
{
	FInventoryItemInfo itemInfo;
	EquipmentComponent->GetWearedEquipmentInof(EquipmentType, itemInfo);
	return itemInfo;
}

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryManagerComp = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>();
	check(InventoryManagerComp);
	
	EquipmentComponent = GetOwningPlayerPawn()->FindComponentByClass<UEquipmentManagerComponent>();
	check(EquipmentComponent);
	EquipmentComponent->OnEquip.AddUObject(this, &ThisClass::HandleEquip);
	EquipmentComponent->OnUnEquip.AddUObject(this, &ThisClass::HandleUnEquip);
}

FReply UEquipmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UEquipmentSlotWidget::HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance)
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

void UEquipmentSlotWidget::HandleUnEquip(EEquipmentType InEquipmentType)
{
	if (InEquipmentType == EquipmentType)
	{
		Image_Icon->SetVisibility(ESlateVisibility::Hidden);
		Image_Icon->SetBrushFromTexture(nullptr);
	}
}

