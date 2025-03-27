// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryManagerComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
}

void UInventoryManagerComponent::AddItem_Implementation(const FInventoryItemInfo& ItemInfo)
{
	bool FinishAdd = false;
	
	for(FInventoryItemSlot& ItemSlot : InventorySlotList.Slots)
	{
		if(ItemSlot.ItemInfo == ItemInfo)
		{
			if(ItemSlot.ItemInfo.CanStack)
			{
				ItemSlot.Amount ++;
				FinishAdd = true;
			}
		}
	}

	if(FinishAdd)
	{
		InventorySlotList.MarkArrayDirty();
	}
	else
	{
		FInventoryItemSlot itemSlot;
		itemSlot.ItemInfo = ItemInfo;
		itemSlot.Amount = 1;
		InventorySlotList.Slots.Add(itemSlot);
		InventorySlotList.MarkArrayDirty();
	}
}


// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UInventoryManagerComponent, InventorySlotList, COND_None, REPNOTIFY_Always);
}

void UInventoryManagerComponent::OnRep_SlotList()
{
	OnInventorySlotListChanged.Broadcast(InventorySlotList);
}


