// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "InventoryList.generated.h"

class UCanvasPanel;
class UEquipmentSlotWidget;
class IItemOperationInterface;
class UEquipmentManagerComponent;
class UInventoryManagerComponent;
enum class EItemOperationType : uint8;
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
	int32 MaxRow = 8;
	
	UPROPERTY(EditAnywhere)
	int32 MaxColumn = 8;
	
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* UGP_InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* EquipmentPanel;
	
	UPROPERTY()
	TArray<UInventorySlot*> InventorySlots;

	UPROPERTY()
	UItemOperationPanel* ItemOperationPanel;

	UPROPERTY()
	UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY()
	UEquipmentManagerComponent* EquipmentManagerComponent;
};
