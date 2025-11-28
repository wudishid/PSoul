// Fill out your copyright notice in the Description page of Project Settings.
#include "Equipment/EquipmentManagerComponent.h"
#include "Equipment/EquipmentInstance.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "Util/Util_Common.h"
#include "Util/Util_Inventory.h"


void FEquipmentSlotList::SetEquipmentSlot(EEquipmentType InEquipmentType, AEquipmentInstance* InEquipmentInstance)
{
	for (FEquipmentSlot& EquipmentSlot : Slots)
	{
		if (EquipmentSlot.EquipmentType == InEquipmentType)
		{
			EquipmentSlot.EquipmentInstance = InEquipmentInstance;
			return;
		}
	}
}

AEquipmentInstance* FEquipmentSlotList::GetEquipmentByType(EEquipmentType InEquipmentType)
{
	for (FEquipmentSlot& EquipmentSlot : Slots)
	{
		if (EquipmentSlot.EquipmentType == InEquipmentType)
		{
			return EquipmentSlot.EquipmentInstance;
		}
	}
	return nullptr;
}

void FEquipmentSlotList::AddSlot(FEquipmentSlot InSlot)
{
	Slots.Add(InSlot);
}

// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
}

void UEquipmentManagerComponent::Drop_Implementation(EEquipmentType InEquipmentType)
{
	if(AEquipmentInstance* Equipment = EquipmentSlotList.GetEquipmentByType(InEquipmentType))
	{
		if(Util_Common::SpawnInventroyItemInstance(GetOwner(), Equipment->GetItemInfo().ItemClass))
		{
			UnEquip(Equipment->GetItemName());
		}
	}
}

bool UEquipmentManagerComponent::GetWearedEquipmentInfo(EEquipmentType InEquipmentType, FInventoryItemInfo& OutItemInfo)
{
	if(AEquipmentInstance* Equipment = EquipmentSlotList.GetEquipmentByType(InEquipmentType))
	{
		OutItemInfo = Equipment->GetItemInfo();
	}
	
	return false;
}

AEquipmentInstance* UEquipmentManagerComponent::GetEquipmentInstance(EEquipmentType InEquipmentType)
{
	return EquipmentSlotList.GetEquipmentByType(InEquipmentType);
}

void UEquipmentManagerComponent::Equip_Implementation(FName InEquipmentName)
{
	FInventoryItemInfo ItemInfo;
	if (Util_Inventory::GetItemInfoByName(InEquipmentName, ItemInfo))
	{
		if (TSubclassOf<AEquipmentInstance> EquipmentClass = ItemInfo.EquipmentClass)
		{
			FActorSpawnParameters Sp;
			Sp.Owner = GetOwner();
			if (AEquipmentInstance* Equipment = GetWorld()->SpawnActor<AEquipmentInstance>(EquipmentClass, Sp))
			{
				EquipmentSlotList.SetEquipmentSlot(Equipment->GetEquipmentType(), Equipment);
				Equipment->Equip();
				OnEquip.Broadcast(Equipment->GetEquipmentType(), Equipment);
			}
		}
	}
}

void UEquipmentManagerComponent::UnEquip_Implementation(FName InEquipmentName)
{
	FInventoryItemInfo ItemInfo;
	if (Util_Inventory::GetItemInfoByName(InEquipmentName, ItemInfo))
	{
		if (TSubclassOf<AEquipmentInstance> EquipmentClass = ItemInfo.EquipmentClass)
		{
			if(AEquipmentInstance* Equipment = EquipmentSlotList.GetEquipmentByType(EquipmentClass->GetDefaultObject<AEquipmentInstance>()->GetEquipmentType()))
			{
				OnUnEquip.Broadcast(Equipment->GetEquipmentType());
				EquipmentSlotList.SetEquipmentSlot(Equipment->GetEquipmentType(), nullptr);
				Equipment->UnEquip();
			}
		}
	}
}

// Called when the game starts
void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		InitEquipmentSlotList();
	}
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, EquipmentSlotList, COND_None, REPNOTIFY_Always);
}


void UEquipmentManagerComponent::InitEquipmentSlotList()
{
	EquipmentSlotList.AddSlot(FEquipmentSlot(EEquipmentType::Weapon, nullptr));
	EquipmentSlotList.AddSlot(FEquipmentSlot(EEquipmentType::Armor, nullptr));
	EquipmentSlotList.AddSlot(FEquipmentSlot(EEquipmentType::Shoes, nullptr));
	EquipmentSlotList.AddSlot(FEquipmentSlot(EEquipmentType::Ring, nullptr));
}


void UEquipmentManagerComponent::OnRep_EquipmentSlotList(const FEquipmentSlotList& OldEquipmentSlotList)
{
	for(int32 index = 0; index < OldEquipmentSlotList.Slots.Num(); index++)
	{
		if(EquipmentSlotList.Slots[index].EquipmentInstance != OldEquipmentSlotList.Slots[index].EquipmentInstance)
		{
			if(EquipmentSlotList.Slots[index].EquipmentInstance)
			{
				OnEquip.Broadcast(EquipmentSlotList.Slots[index].EquipmentType, EquipmentSlotList.Slots[index].EquipmentInstance);
			}
			else
			{
				OnUnEquip.Broadcast(EquipmentSlotList.Slots[index].EquipmentType);
			}
		}
	}
}


