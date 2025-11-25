// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Inventory/InventoryItemDefinition.h"


namespace Util_Inventory
{
	bool GetItemInfoByName(FName name, FInventoryItemInfo& outItemInfo);
	FString ItemOperationTypeToString(EItemOperationType type);
}
