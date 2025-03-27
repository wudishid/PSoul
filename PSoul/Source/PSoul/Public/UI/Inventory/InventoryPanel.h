// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "InventoryPanel.generated.h"

struct FInventoryItemSlotList;
class UInventorySlot;
class UUniformGridPanel;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UInventoryPanel : public USoulUserWidget
{
	GENERATED_BODY()

	void OnPlayerInventoryChanged(FInventoryItemSlotList& InventoryItemSlotList);
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void CreateInventoryPanel();
	
	UPROPERTY(EditAnywhere)
	int32 MaxColumn = 5;
	
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* UGP_InventoryPanel;
	
	TArray<UInventorySlot> InventorySlots;
};
