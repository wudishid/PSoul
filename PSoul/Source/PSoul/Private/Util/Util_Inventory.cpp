// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/Util_Inventory.h"

bool Util_Inventory::GetItemInfoByName(FName name, FInventoryItemInfo& outItemInfo)
{
	FSoftObjectPath ItemTablePath(TEXT("/Game/Data/Inventory/DT_InventoryItem.DT_InventoryItem"));
	if(UDataTable* ItemTable = Cast<UDataTable>(ItemTablePath.TryLoad()))
	{
		if(FInventoryItemInfo* ItemInfo = ItemTable->FindRow<FInventoryItemInfo>(name, TEXT("item")))
		{
			outItemInfo = *ItemInfo;
			return true;
		}
	}
	
	return false;
}

FString Util_Inventory::ItemOperationTypeToString(EItemOperationType type)
{
	switch (type)
	{
	case EItemOperationType::Use:
		return TEXT("使用");
	case EItemOperationType::Drop:
		return TEXT("丢弃");
	case EItemOperationType::Equip:
		return TEXT("装备");
	case EItemOperationType::UnEquip:
		return TEXT("卸下");
	default:
		return TEXT("");
	}
}
