// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemOperationInterface.generated.h"

struct FInventoryItemInfo;
enum class EItemOpetaionType : uint8;
// This class does not need to be modified.
UINTERFACE()
class UItemOperationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PSOUL_API IItemOperationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual  EItemOpetaionType GetRulesForOperationType(EItemOpetaionType type) = 0;
	virtual  void HandleItemOperation(EItemOpetaionType OpetaionType) = 0;
	virtual FInventoryItemInfo GetItemInfo() const = 0;
};
