// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemOperationInterface.h"
#include "UI/SoulUserWidget.h"
#include "InventorySlot.generated.h"

class UEquipmentManagerComponent;
class UInventoryManagerComponent;
struct FInventoryItemSlot;
class UButton;
class UTextBlock;
class UImage;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotRightMouseButtonDown, IItemOperationInterface*,  /**位置**/FVector2d);

UCLASS(Abstract)
class PSOUL_API UInventorySlot : public USoulUserWidget, public IItemOperationInterface
{
	GENERATED_BODY()

public:
	void UpdateSlot();
	bool IsEmpty() const;
	int32 SlotIndex;

	FOnSlotRightMouseButtonDown OnSlotRightMouseButtonDown;
public:
	virtual EItemOperationType GetRulesForOperationType(EItemOperationType type) override;
	virtual void HandleItemOperation(EItemOperationType OpetaionType) override;
	virtual FInventoryItemInfo GetItemInfo() const override;
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Amount;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Slot;

	bool bEmpty;
	
	UPROPERTY()
	UInventoryManagerComponent* InventoryManagerComp;

	UPROPERTY()
	UEquipmentManagerComponent* EquipmentManagerComp;
};
