// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/InventoryManagerComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "PSoul/SoulLog.h"
#include "Util/Util_Common.h"
#include "Util/Util_Inventory.h"

#define BROADCAST_LISTCHANGED_IF_STANDALONE \
if (GetNetMode() == NM_Standalone) \
{ \
	OnInventorySlotListChanged.Broadcast(InventorySlotList); \
}

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
}

void UInventoryManagerComponent::RemoveItem_Implementation(int32 InItemIndex)
{
	if(InventorySlotList.Slots.IsValidIndex(InItemIndex))
	{
		InventorySlotList.Slots.RemoveAt(InItemIndex);
		InventorySlotList.MarkArrayDirty();
		BROADCAST_LISTCHANGED_IF_STANDALONE
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

void UInventoryManagerComponent::UseItem_Implementation(int32 InItemIndex, int32 InUseAmount)
{
	if(InventorySlotList.Slots.IsValidIndex(InItemIndex))
	{
		TSubclassOf<UGameplayEffect>ItemEffectClass =  InventorySlotList.Slots[InItemIndex].ItemInfo.ItemClass.GetDefaultObject()->GetItemEffectClass();
		ASC->ApplyGameplayEffectToSelf(ItemEffectClass.GetDefaultObject(), 1, ASC->MakeEffectContext());
		if(--InventorySlotList.Slots[InItemIndex].Amount <=0)
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

void UInventoryManagerComponent::AddItem_Implementation(FName InItemName, int32 InAmount)
{
	bool bFinishAdd = false;

	FInventoryItemInfo ItemInfo;
	Util_Inventory::GetItemInfoByName(InItemName, ItemInfo);
	if (!ensureAlways(ItemInfo.IsValid())) { return; }
	
	for(FInventoryItemSlot& ItemSlot : InventorySlotList.Slots)
	{
		if(ItemSlot.ItemInfo == ItemInfo)
		{
			if(ItemSlot.ItemInfo.CanStack)
			{
				int32 SumAmount = ItemSlot.Amount + InAmount;
				if (SumAmount > ItemSlot.ItemInfo.MaxStackAmount)
				{
					ItemSlot.Amount = ItemSlot.ItemInfo.MaxStackAmount;
					AddItem(InItemName, SumAmount - ItemSlot.ItemInfo.MaxStackAmount);
				}
				else
				{
					ItemSlot.Amount = SumAmount;
					bFinishAdd = true;
				}
			}
		}
	}

	if(bFinishAdd)
	{
		InventorySlotList.MarkArrayDirty();
		BROADCAST_LISTCHANGED_IF_STANDALONE
	}
	else
	{
		FInventoryItemSlot itemSlot;
		itemSlot.ItemInfo = ItemInfo;
		itemSlot.Amount = InAmount;
		InventorySlotList.Slots.Add(itemSlot);
		InventorySlotList.MarkArrayDirty();
		BROADCAST_LISTCHANGED_IF_STANDALONE
	}
}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ASC = GetOwner()->FindComponentByClass<USoulAbilitySystemComponent>();
	check(ASC);
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

int32 UInventoryManagerComponent::GetSlotIndex(const FInventoryItemInfo& ItemInfo) const
{
	for(int i = 0; i < InventorySlotList.Slots.Num(); i++)
	{
		if(InventorySlotList.Slots[i].ItemInfo == ItemInfo)
		{
			return i;
		}
	}
	
	return -1;
}


