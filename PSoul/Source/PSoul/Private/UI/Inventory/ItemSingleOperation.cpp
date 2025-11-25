// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/ItemSingleOperation.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Util/Util_Inventory.h"


void UItemSingleOperation::UpdateSingleOperation(EItemOperationType OperationType)
{
	ItemOpetaionType = OperationType;
	Text_OperationName->SetText(FText::FromString(Util_Inventory::ItemOperationTypeToString(ItemOpetaionType)));
}

void UItemSingleOperation::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Operate->OnClicked.AddDynamic(this, &ThisClass::HandleBtn_OperateClicked);
}

void UItemSingleOperation::HandleBtn_OperateClicked()
{
	OnOperationClicked.Broadcast(ItemOpetaionType);
}
