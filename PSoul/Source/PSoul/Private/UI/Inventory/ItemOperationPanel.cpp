// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/ItemOperationPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Development/Soul_UISetting.h"
#include "Inventory/InventoryManagerComponent.h"
#include "PSoul/SoulLog.h"
#include "UI/Inventory/ItemSingleOperation.h"

void UItemOperationPanel::UpdateOperationPanel(int32 SlotIndex,  FVector2d Position)
{
	OperateSlotIndex = SlotIndex;

	if(UCanvasPanelSlot* CanvasPanelSlotSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(VB_OperationList))
	{
		CanvasPanelSlotSlot->SetPosition(Position);
	}
	
	VB_OperationList->ClearChildren();
	
	if(UInventoryManagerComponent* InventoryManagerComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManagerComponent>())
	{
		FInventoryItemInfo OperateItemInfo;
		if(InventoryManagerComponent->GetItemInfoByIndex(OperateSlotIndex, OperateItemInfo))
		{
			for(EItemOpetaionType OperationType : OperateItemInfo.ItemOpetaions)
			{
				if(UItemSingleOperation* SingleOperation = CreateWidget<UItemSingleOperation>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->ItemSingleOperationClass.LoadSynchronous()))
				{
					SingleOperation->UpdateSingleOperation(OperationType);
					SingleOperation->OnOperationClicked.AddUObject(this, &ThisClass::HandleSIngleOperationClicked);
					VB_OperationList->AddChildToVerticalBox(SingleOperation);
				}
			}
		}
		else
		{
			UE_LOG(LogSoulInventory, Error, TEXT("Not Find Item By Index"));
		}
	}
}

void UItemOperationPanel::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetIsFocusable(true);
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

void UItemOperationPanel::HandleSIngleOperationClicked(EItemOpetaionType OperationType)
{
	OnItemOperationClicked.Broadcast(OperateSlotIndex, OperationType);
	VB_OperationList->ClearChildren();
	RemoveFromParent();
}
