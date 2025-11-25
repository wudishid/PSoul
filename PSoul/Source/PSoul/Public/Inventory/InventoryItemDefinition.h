// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryItemDefinition.generated.h"

class AEquipmentInstance;
class AInventoryItemInstance;


UENUM()
enum class EItemType : uint8
{
	Consumable = 0,
	Equipment
};


UENUM()
enum class EEquipmentType : uint8
{
	Weapon,
	Armor,
	Shield,
	Ring
};

UENUM()
enum class EItemOperationType : uint8
{
	None = 0,
	Use,
	Drop,
	Equip,
	UnEquip
};

USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	FInventoryItemInfo() :  ItemName(NAME_None), Description(), Icon(nullptr), CanStack(false), MaxStackAmount(1)
	{
	}

	bool IsValid() const { return !ItemName.IsNone(); }

	bool operator==(const FInventoryItemInfo& Other) const { return IsValid() && ItemName == Other.ItemName; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Consumable"))
	bool CanStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "CanStack"))
	int32 MaxStackAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::Consumable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EItemOperationType> ItemOperations;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventoryItemInstance> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Equipment"))
	TSubclassOf<AEquipmentInstance> EquipmentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Equipment"))
	EEquipmentType EquipmentType = EEquipmentType::Weapon;
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
		return HashCombine(GetTypeHash(Amount), GetTypeHash(ItemInfo.ItemName));
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
		return FastArrayDeltaSerialize<FInventoryItemSlot, FInventoryItemSlotList>(Slots, DeltaParms, *this);
	}
	
	UPROPERTY()
	TArray<FInventoryItemSlot> Slots;
};

