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
