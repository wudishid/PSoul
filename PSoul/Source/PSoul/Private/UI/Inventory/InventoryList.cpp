// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/InventoryList.h"

#include <filesystem>

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Development/Soul_UISetting.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "UI/Equipment/EquipmentSlot.h"
#include "UI/Inventory/InventorySlot.h"
#include "UI/Inventory/ItemOperationPanel.h"

void UInventoryList::OnPlayerInventoryChanged(FInventoryItemSlotList& InventoryItemSlotList)
{
	CreateInventoryPanel();
}

void UInventoryList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (GetWorld() && !GetWorld()->HasBegunPlay())
	{
		CreateInventoryPanel();
	}
}

void UInventoryList::NativeConstruct()
{
	Super::NativeConstruct();
	
	InventoryManagerComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>();
	check(InventoryManagerComponent);
	InventoryManagerComponent->OnInventorySlotListChanged.AddUObject(this, &ThisClass::OnPlayerInventoryChanged);

	EquipmentManagerComponent = GetOwningPlayerPawn()->FindComponentByClass<UEquipmentManagerComponent>();
	check(EquipmentManagerComponent);

	WeaponSlot->OnSlotRightMouseButtonDown.AddUObject(this, &ThisClass::HandleSlotRightMouseButtonDown);
	ArmorSlot->OnSlotRightMouseButtonDown.AddUObject(this, &ThisClass::HandleSlotRightMouseButtonDown);
	RingSlot->OnSlotRightMouseButtonDown.AddUObject(this, &ThisClass::HandleSlotRightMouseButtonDown);
	
	CreateInventoryPanel();
}

void UInventoryList::CreateInventoryPanel()
{
	UGP_InventoryPanel->ClearChildren();
	
	for(int index = 0; index < 30; index++)
	{
		UInventorySlot* InventorySlot =nullptr;
		if(GetWorld()->HasBegunPlay())
		{
			InventorySlot = CreateWidget<UInventorySlot>(GetOwningPlayer(),  GetDefault<USoul_UISetting>()->InventorySlotClass.LoadSynchronous());
			InventorySlot->SlotIndex = index;
			InventorySlot->UpdateSlot();
			InventorySlot->OnSlotRightMouseButtonDown.AddUObject(this, &ThisClass::HandleSlotRightMouseButtonDown);
		}
		else
		{
			InventorySlot = CreateWidget<UInventorySlot>(this, GetDefault<USoul_UISetting>()->InventorySlotClass.LoadSynchronous());
		}
		
		int32 TempRow = index / MaxColumn;
		int32 TempColumn = index % MaxColumn;
		UGP_InventoryPanel->AddChildToUniformGrid(InventorySlot, TempRow, TempColumn );
	}
}

void UInventoryList::HandleSlotRightMouseButtonDown(IItemOperationInterface* InOperatedSlot, FVector2d InPosition)
{
	if(ItemOperationPanel)
	{
		if(!ItemOperationPanel->IsInViewport())
		{
			ItemOperationPanel->AddToViewport();
			ItemOperationPanel->UpdateOperationPanel(InOperatedSlot, InPosition);
		}
	}
	else
	{
		ItemOperationPanel = CreateWidget<UItemOperationPanel>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->ItemOperationPanelClass.LoadSynchronous());
		if(ensure(ItemOperationPanel))
		{
			ItemOperationPanel->AddToViewport();
			ItemOperationPanel->UpdateOperationPanel(InOperatedSlot, InPosition);
		}
	}
}
