// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "InventoryList.generated.h"

class UEquipmentSlot;
class IItemOperationInterface;
class UEquipmentManagerComponent;
class UInventoryManagerComponent;
enum class EItemOpetaionType : uint8;
class UItemOperationPanel;
struct FInventoryItemSlotList;
class UInventorySlot;
class UUniformGridPanel;


UCLASS(Abstract)
class PSOUL_API UInventoryList : public USoulUserWidget
{
	GENERATED_BODY()
	
	void OnPlayerInventoryChanged(FInventoryItemSlotList& InventoryItemSlotList);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void CreateInventoryPanel();

	void HandleSlotRightMouseButtonDown(IItemOperationInterface* InOperatedSlot, FVector2d InPosition);
	
	UPROPERTY(EditAnywhere)
	int32 MaxColumn = 5;
	
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* UGP_InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	UEquipmentSlot* WeaponSlot;

	UPROPERTY(meta = (BindWidget))
	UEquipmentSlot* ArmorSlot;

	UPROPERTY(meta = (BindWidget))
	UEquipmentSlot* RingSlot;
	
	UPROPERTY()
	TArray<UInventorySlot*> InventorySlots;

	UPROPERTY()
	UItemOperationPanel* ItemOperationPanel;

	UPROPERTY()
	UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY()
	UEquipmentManagerComponent* EquipmentManagerComponent;
};
