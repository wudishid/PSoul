// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Equipment/EquipmentSlotWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Equipment/EquipmentInstance.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"


void UEquipmentSlotWidget::Init()
{
	InventoryManagerComp = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>();
	check(InventoryManagerComp);
	
	EquipmentComponent = GetOwningPlayerPawn()->FindComponentByClass<UEquipmentManagerComponent>();
	check(EquipmentComponent);
	EquipmentComponent->OnEquip.AddUObject(this, &ThisClass::HandleEquip);
	EquipmentComponent->OnUnEquip.AddUObject(this, &ThisClass::HandleUnEquip);

	if (AEquipmentInstance* EquipmentInstance = EquipmentComponent->GetEquipmentInstance(EquipmentType))
	{
		Image_Icon->SetVisibility(ESlateVisibility::Visible);
		Image_Icon->SetBrushFromTexture(EquipmentInstance->GetItemInfo().Icon.LoadSynchronous());
	}
	else
	{
		Image_Icon->SetVisibility(ESlateVisibility::Hidden);
		Image_Icon->SetBrushFromTexture(nullptr);
	}
}

EItemOperationType UEquipmentSlotWidget::GetRulesForOperationType(EItemOperationType type)
{
	if(type == EItemOperationType::Equip)
	{
		return EItemOperationType::None;
	}
	return type;
}

void UEquipmentSlotWidget::HandleItemOperation(EItemOperationType OpetaionType)
{
	if(OpetaionType == EItemOperationType::Drop)
	{
		EquipmentComponent->Drop(EquipmentType);
	}
	else if(OpetaionType == EItemOperationType::UnEquip)
	{
		InventoryManagerComp->AddItem(GetItemInfo().ItemName);
		EquipmentComponent->UnEquip(GetItemInfo().ItemName);
	}
}

FInventoryItemInfo UEquipmentSlotWidget::GetItemInfo() const
{
	FInventoryItemInfo itemInfo;
	EquipmentComponent->GetWearedEquipmentInfo(EquipmentType, itemInfo);
	return itemInfo;
}

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
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

