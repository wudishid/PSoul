// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemDefinition.h"
#include "Components/ActorComponent.h"
#include "InventoryManagerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotListChanged, FInventoryItemSlotList&);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryManagerComponent();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddItem(const FInventoryItemInfo& ItemInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RemoveItem(int32 InItemIndex);

	UFUNCTION(Server, Reliable)
	void DropItem(int32 InItemIndex);
	
	/// @return 是否找到物品信息
	bool GetItemInfoByIndex(int32 Index, FInventoryItemInfo& OutItemInfo) const;

	bool GetItemSlotByIndex(int32 Index, FInventoryItemSlot& OutItemSlot) const;
	
	FOnInventorySlotListChanged OnInventorySlotListChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_SlotList();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_SlotList)
	FInventoryItemSlotList InventorySlotList;
};
