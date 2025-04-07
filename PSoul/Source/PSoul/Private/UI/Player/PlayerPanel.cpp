// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Player/PlayerPanel.h"
#include "UI/Character/StateBar.h"
#include "UI/Inventory/InventoryList.h"


void UPlayerPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerPanel::InitPanel()
{
	HealthBar->Init(GetOwningPlayerPawn());
	StaminaBar->Init(GetOwningPlayerPawn());
	SetShowInventoryPanel(false);
}

void UPlayerPanel::SetShowInventoryPanel(bool bShow)
{
	if (bShow)
	{
		InventoryList->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InventoryList->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UPlayerPanel::IsShowInventoryPanel() const
{
	return InventoryList->IsVisible();
}
