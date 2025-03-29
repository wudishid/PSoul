// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "ItemOperationPanel.generated.h"

class UItemSingleOperation;
enum class EItemOpetaionType : uint8;
class UVerticalBox;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemOperationClicked, /**物品索引**/int32, EItemOpetaionType);

UCLASS(Abstract)
class PSOUL_API UItemOperationPanel : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void UpdateOperationPanel(int32 SlotIndex, FVector2d Position);

	FOnItemOperationClicked OnItemOperationClicked;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION()
	void HandleSIngleOperationClicked(EItemOpetaionType OperationType);
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_OperationList;
	
	int32 OperateSlotIndex;
};
