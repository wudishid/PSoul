// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemDefinition.h"
#include "Components/ActorComponent.h"
#include "InventoryManagerComponent.generated.h"

class USoulAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotListChanged, FInventoryItemSlotList&);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryManagerComponent();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddItem(FName InItemName, int32 InAmount = 1);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RemoveItem(int32 InItemIndex);

	UFUNCTION(Server, Reliable)
	void DropItem(int32 InItemIndex);

	UFUNCTION(Server, Reliable)
	void UseItem(int32 InItemIndex, int32 InUseAmount = 1);
	
	bool GetItemInfoByIndex(int32 Index, FInventoryItemInfo& OutItemInfo) const;

	bool GetItemSlotByIndex(int32 Index, FInventoryItemSlot& OutItemSlot) const;
	
	FInventoryItemSlotList* GetInventoryItemSlotList() { return &InventorySlotList; }
	
	FOnInventorySlotListChanged OnInventorySlotListChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_SlotList();

	int32 GetSlotIndex(const FInventoryItemInfo& ItemInfo) const;
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_SlotList)
	FInventoryItemSlotList InventorySlotList;

	UPROPERTY()
	USoulAbilitySystemComponent* ASC =nullptr;
};
