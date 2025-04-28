// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "UI/Inventory/ItemOperationInterface.h"
#include "EquipmentSlotWidget.generated.h"

class UInventoryManagerComponent;
class UEquipmentManagerComponent;
class UImage;
class AEquipmentInstance;
enum class EEquipmentType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotRightMouseButtonDown, IItemOperationInterface*,  /**位置**/FVector2d);

UCLASS(Abstract)
class PSOUL_API UEquipmentSlotWidget : public USoulUserWidget, public IItemOperationInterface
{
	GENERATED_BODY()

public:
	virtual EItemOpetaionType GetRulesForOperationType(EItemOpetaionType type) override;
	virtual void HandleItemOperation(EItemOpetaionType OpetaionType) override;
	virtual FInventoryItemInfo GetItemInfo() const override;

	FOnSlotRightMouseButtonDown OnSlotRightMouseButtonDown;
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance);
	void HandleUnEquip(EEquipmentType InEquipmentType);

	UPROPERTY(meta=(BindWidget))
	UImage* Image_Icon;
	
	UPROPERTY(EditAnywhere, Category = "EquipmentSlot")
	EEquipmentType EquipmentType;

	UPROPERTY()
	UInventoryManagerComponent* InventoryManagerComp;
	
	UPROPERTY()
	UEquipmentManagerComponent* EquipmentComponent;
};
