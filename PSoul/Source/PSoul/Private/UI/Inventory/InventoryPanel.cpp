// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/InventoryPanel.h"

#include "Components/UniformGridPanel.h"
#include "Inventory/InventoryManagerComponent.h"
#include "UI/Inventory/InventorySlot.h"

void UInventoryPanel::OnPlayerInventoryChanged(FInventoryItemSlotList& InventoryItemSlotList)
{
	UGP_InventoryPanel->ClearChildren();
	
	FSoftClassPath SoftClassPath(TEXT("/Game/UI/Inventory/WBP_InventorySlot.WBP_InventorySlot_C"));
	UClass* SlotClass = SoftClassPath.TryLoadClass<UInventorySlot>();
	check(SlotClass);
	
	for(int index = 0; index < 30; index++)
	{
		UInventorySlot* InventorySlot =nullptr;
		if(GetWorld()->HasBegunPlay())
		{
			InventorySlot = CreateWidget<UInventorySlot>(GetOwningPlayer(), SlotClass);
		}
		else
		{
			InventorySlot = CreateWidget<UInventorySlot>(this, SlotClass);
		}
		if(InventoryItemSlotList.Slots.IsValidIndex(index))
		{
			InventorySlot->UpdateSlot(InventoryItemSlotList.Slots[index]);
		}
		int32 TempRow = index / MaxColumn;
		int32 TempColumn = index % MaxColumn;
		UGP_InventoryPanel->AddChildToUniformGrid(InventorySlot, TempRow, TempColumn );
	}
}

void UInventoryPanel::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(!GetWorld()->HasBegunPlay())
	{
		CreateInventoryPanel();
	}
}

void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	CreateInventoryPanel();

	if(UInventoryManagerComponent* InventoryManagerComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>())
	{
		InventoryManagerComponent->OnInventorySlotListChanged.AddUObject(this, &ThisClass::OnPlayerInventoryChanged);
	}
}

void UInventoryPanel::CreateInventoryPanel()
{
	UGP_InventoryPanel->ClearChildren();
	
	FSoftClassPath SoftClassPath(TEXT("/Game/UI/Inventory/WBP_InventorySlot.WBP_InventorySlot_C"));
	UClass* SlotClass = SoftClassPath.TryLoadClass<UInventorySlot>();
	check(SlotClass);
	
	for(int index = 0; index < 30; index++)
	{
		UInventorySlot* InventorySlot =nullptr;
		if(GetWorld()->HasBegunPlay())
		{
			InventorySlot = CreateWidget<UInventorySlot>(GetOwningPlayer(), SlotClass);
		}
		else
		{
			InventorySlot = CreateWidget<UInventorySlot>(this, SlotClass);
		}
		
		int32 TempRow = index / MaxColumn;
		int32 TempColumn = index % MaxColumn;
		UGP_InventoryPanel->AddChildToUniformGrid(InventorySlot, TempRow, TempColumn );
	}
}
