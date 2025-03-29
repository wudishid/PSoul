// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentManagerComponent.h"

#include "Equipment/EquipmentInstance.h"
#include "Inventory/InventoryItemDefinition.h"


// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
}

void UEquipmentManagerComponent::WearEquipment_Implementation(TSubclassOf<AEquipmentInstance> EquipmentClass)
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
				break;
			case EEquipmentType::Armor:
				Armor = Equipment;
				break;
			case EEquipmentType::Ring:
				Ring = Equipment;
				break;
			}
		}
	}
}

void UEquipmentManagerComponent::RemoveEquipment_Implementation(TSubclassOf<AEquipmentInstance> EquipmentClass)
{
	if (EquipmentClass)
	{
		switch (EquipmentClass.GetDefaultObject()->GetEquipmentType())
		{
		case EEquipmentType::Weapon:
			Weapon->UnEquip();
			Weapon = nullptr;
			break;
		case EEquipmentType::Armor:
			Armor->UnEquip();
			Armor = nullptr;
			break;
		case EEquipmentType::Ring:
			Ring->UnEquip();
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
}


