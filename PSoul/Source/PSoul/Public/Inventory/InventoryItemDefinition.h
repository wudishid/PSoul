// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryItemDefinition.generated.h"

class AInventoryItemInstance;

USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	FInventoryItemInfo() :  ItemName(NAME_None), Description(), Icon(nullptr), CanStack(false)
	{
	}

	bool IsValid() const { return Guid.IsValid(); }

	bool operator==(const FInventoryItemInfo& Other) const { return Guid == Other.Guid; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid Guid;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanStack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventoryItemInstance> ItemClass;
	
};


USTRUCT(BlueprintType)
struct FInventoryItemSlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryItemSlot() : Amount(1)
	{
	}

	bool IsValidSlot() const
	{
		return ItemInfo.IsValid();
	}
	
	int32 GetItemHash() const
	{
		return HashCombine(GetTypeHash(Amount), GetTypeHash(ItemInfo.Guid));
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryItemInfo ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
	
};

USTRUCT(BlueprintType)
struct FInventoryItemSlotList : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryItemSlot, FInventoryItemSlotList>(Slots, DeltaParms, *this);
	}
	
	UPROPERTY()
	TArray<FInventoryItemSlot> Slots;
};

