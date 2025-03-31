// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/InventoryManagerComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "Util/Util_Common.h"


// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
}

void UInventoryManagerComponent::RemoveItem_Implementation(int32 InItemIndex)
{
	if(InventorySlotList.Slots.IsValidIndex(InItemIndex))
	{
		InventorySlotList.Slots.RemoveAt(InItemIndex);
	}
}

void UInventoryManagerComponent::DropItem_Implementation(int32 InItemIndex)
{
	if(InventorySlotList.Slots.IsValidIndex(InItemIndex))
	{
		TSubclassOf<AInventoryItemInstance> ItemInstanceClass = InventorySlotList.Slots[InItemIndex].ItemInfo.ItemClass;
		if(Util_Common::SpawnInventroyItemInstance(GetOwner(), ItemInstanceClass))
		{
			RemoveItem(InItemIndex);
		}
	}
}

bool UInventoryManagerComponent::GetItemInfoByIndex(int32 Index, FInventoryItemInfo& OutItemInfo) const
{
	if(InventorySlotList.Slots.IsValidIndex(Index))
	{
		OutItemInfo = InventorySlotList.Slots[Index].ItemInfo;
		return true;
	}

	return false;
}

bool UInventoryManagerComponent::GetItemSlotByIndex(int32 Index, FInventoryItemSlot& OutItemSlot) const
{
	if(InventorySlotList.Slots.IsValidIndex(Index))
	{
		OutItemSlot = InventorySlotList.Slots[Index];
		return true;
	}

	return false;
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


