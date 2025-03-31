// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentManagerComponent.h"

#include "Equipment/EquipmentInstance.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Util/Util_Common.h"


// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
}

void UEquipmentManagerComponent::Drop_Implementation(EEquipmentType InEquipmentType)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		if(Weapon)
		{
			if(Util_Common::SpawnInventroyItemInstance(GetOwner(), Weapon->GetItemInfo().ItemClass))
			{
				Weapon->UnEquip();
				Weapon = nullptr;
			}
		}
	}
	else if(InEquipmentType == EEquipmentType::Armor)
	{
		if(Armor)
		{
			if(Util_Common::SpawnInventroyItemInstance(GetOwner(), Armor->GetItemInfo().ItemClass))
			{
				Armor->UnEquip();
				Armor = nullptr;
			}
		}
	}
	else if(InEquipmentType == EEquipmentType::Ring)
	{
		if(Ring)
		{
			if(Util_Common::SpawnInventroyItemInstance(GetOwner(), Weapon->GetItemInfo().ItemClass))
			{
				Ring->UnEquip();
				Ring = nullptr;
			}
		}
	}
}

bool UEquipmentManagerComponent::GetWearedEquipmentInof(EEquipmentType InEquipmentType, FInventoryItemInfo& OutItemInfo)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		if(Weapon)
		{
			OutItemInfo = Weapon->GetItemInfo();
			return true;
		}
	}
	else if(InEquipmentType == EEquipmentType::Armor)
	{
		if(Armor)
		{
			OutItemInfo = Armor->GetItemInfo();
			return true;
		}
	}
	else if(InEquipmentType == EEquipmentType::Ring)
	{
		if(Ring)
		{
			OutItemInfo = Ring->GetItemInfo();
			return true;
		}
	}
	
	return false;
}

AEquipmentInstance* UEquipmentManagerComponent::GetEquipmentInstance(EEquipmentType InEquipmentType)
{
	switch (InEquipmentType)
	{
	case EEquipmentType::Weapon:
		return Weapon;
	case EEquipmentType::Armor:
		return Armor;
	case EEquipmentType::Ring:
		return Ring;
	default:
		return nullptr;
	}
}

void UEquipmentManagerComponent::Equip_Implementation(TSubclassOf<AEquipmentInstance> EquipmentClass)
{
	if (EquipmentClass)
	{
		FActorSpawnParameters Sp;
		Sp.Owner = GetOwner();
		if (AEquipmentInstance* Equipment = GetWorld()->SpawnActor<AEquipmentInstance>(EquipmentClass, Sp))
		{
			Equipment->Equip();
			switch (Equipment->GetEquipmentType())
			{
			case EEquipmentType::Weapon:
				Weapon = Equipment;
				OnEquip.Broadcast(EEquipmentType::Weapon, Weapon);
				break;
			case EEquipmentType::Armor:
				Armor = Equipment;
				OnEquip.Broadcast(EEquipmentType::Armor, Armor);
				break;
			case EEquipmentType::Ring:
				Ring = Equipment;
				OnEquip.Broadcast(EEquipmentType::Ring, Ring);
				break;
			}
		}
	}
}

void UEquipmentManagerComponent::UnEquip_Implementation(TSubclassOf<AEquipmentInstance> EquipmentClass)
{
	if (EquipmentClass)
	{
		switch (EquipmentClass.GetDefaultObject()->GetEquipmentType())
		{
		case EEquipmentType::Weapon:
			Weapon->UnEquip();
			OnUnEquip.Broadcast(EEquipmentType::Weapon);
			Weapon = nullptr;
			break;
		case EEquipmentType::Armor:
			Armor->UnEquip();
			OnUnEquip.Broadcast(EEquipmentType::Armor);
			Armor = nullptr;
			break;
		case EEquipmentType::Ring:
			Ring->UnEquip();
			OnUnEquip.Broadcast(EEquipmentType::Ring);
			Ring = nullptr;
			break;
		}
	}
}

// Called when the game starts
void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEquipmentManagerComponent, Weapon, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEquipmentManagerComponent, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEquipmentManagerComponent, Ring, COND_None, REPNOTIFY_Always);
}

void UEquipmentManagerComponent::OnRep_Weapon()
{
	if(Weapon != nullptr)
	{
		OnEquip.Broadcast(EEquipmentType::Weapon, Weapon);
	}
	else
	{
		OnUnEquip.Broadcast(EEquipmentType::Weapon);
	}
}

void UEquipmentManagerComponent::OnRep_Armor()
{
	if(Weapon != nullptr)
	{
		OnEquip.Broadcast(EEquipmentType::Armor, Armor);
	}
	else
	{
		OnUnEquip.Broadcast(EEquipmentType::Armor);
	}
}

void UEquipmentManagerComponent::OnRep_Ring()
{
	if(Weapon != nullptr)
	{
		OnEquip.Broadcast(EEquipmentType::Ring, Ring);
	}
	else
	{
		OnUnEquip.Broadcast(EEquipmentType::Ring);
	}
}


