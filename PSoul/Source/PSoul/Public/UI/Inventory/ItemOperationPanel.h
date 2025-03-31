// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "ItemOperationPanel.generated.h"

class IItemOperationInterface;
class UItemSingleOperation;
enum class EItemOpetaionType : uint8;
class UVerticalBox;

UCLASS(Abstract)
class PSOUL_API UItemOperationPanel : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void UpdateOperationPanel(IItemOperationInterface* InOperatedSlot, FVector2d Position);
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION()
	void HandleSIngleOperationClicked(EItemOpetaionType OperationType);
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_OperationList;
	
	IItemOperationInterface* OperatedSlot;
};
