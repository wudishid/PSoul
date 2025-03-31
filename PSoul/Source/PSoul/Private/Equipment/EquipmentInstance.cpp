// Fill out your copyright notice in the Description page of Project Settings.
#include "Equipment/EquipmentInstance.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Util/Util_Inventory.h"


// Sets default values
AEquipmentInstance::AEquipmentInstance()
{
	bReplicates = true;
}

void AEquipmentInstance::Equip()
{
	EffectHandle = ASC->ApplyGameplayEffectToSelf(EffectToApplyWhenEquiped.GetDefaultObject(), 1, ASC->MakeEffectContext());
}

void AEquipmentInstance::UnEquip()
{
	ASC->RemoveActiveGameplayEffect(EffectHandle);
}

// Called when the game starts or when spawned
void AEquipmentInstance::BeginPlay()
{
	Super::BeginPlay();
	Util_Inventory::GetItemInfoByName(ItemName, ItemInfo);

	ASC = GetOwner()->FindComponentByClass<USoulAbilitySystemComponent>();
	check(ASC);
}

