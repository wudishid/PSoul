// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/ItemOperationPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Development/Soul_UISetting.h"
#include "Inventory/InventoryManagerComponent.h"
#include "UI/Inventory/ItemOperationInterface.h"
#include "UI/Inventory/ItemSingleOperation.h"

void UItemOperationPanel::UpdateOperationPanel(IItemOperationInterface* InOperatedSlot,  FVector2d Position)
{
	OperatedSlot = InOperatedSlot;

	if(UCanvasPanelSlot* CanvasPanelSlotSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(VB_OperationList))
	{
		CanvasPanelSlotSlot->SetPosition(Position);
	}
	
	VB_OperationList->ClearChildren();

	for(EItemOperationType OperationType :OperatedSlot->GetItemInfo().ItemOperations)
	{
		if(UItemSingleOperation* SingleOperation = CreateWidget<UItemSingleOperation>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->ItemSingleOperationClass.LoadSynchronous()))
		{
			SingleOperation->UpdateSingleOperation(OperatedSlot->GetRulesForOperationType(OperationType));
			SingleOperation->OnOperationClicked.AddUObject(this, &ThisClass::HandleSIngleOperationClicked);
			VB_OperationList->AddChildToVerticalBox(SingleOperation);
		}
	}
}

void UItemOperationPanel::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UItemOperationPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UItemOperationPanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		VB_OperationList->ClearChildren();
		RemoveFromParent();
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UItemOperationPanel::HandleSIngleOperationClicked(EItemOperationType OperationType)
{
	if(OperatedSlot)
	{
		OperatedSlot->HandleItemOperation(OperationType);
	}
	VB_OperationList->ClearChildren();
	RemoveFromParent();
}
