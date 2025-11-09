// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Player/PlayerPanel.h"
#include "UI/Character/StateBar.h"
#include "UI/Inventory/InventoryList.h"
#include "UI/Player/Attribute/AttributePanel.h"
#include "UI/SkillTree/SkillTreePanel.h"


void UPlayerPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerPanel::InitPanel()
{
	HealthBar->Init(GetOwningPlayerPawn());
	StaminaBar->Init(GetOwningPlayerPawn());
	SetShowInventoryPanel(false);
	SetShowSkillTreePanel(false);
}

void UPlayerPanel::SetShowInventoryPanel(bool bShow)
{
	if (bShow)
	{
		InventoryList->SetVisibility(ESlateVisibility::Visible);
		AttributePanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InventoryList->SetVisibility(ESlateVisibility::Hidden);
		AttributePanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerPanel::SetShowSkillTreePanel(bool bShow)
{
	if (bShow)
	{
		SkillTreePanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SkillTreePanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UPlayerPanel::IsShowInventoryPanel() const
{
	return InventoryList->IsVisible();
}

bool UPlayerPanel::IsShowSkillTreePanel() const
{
	return SkillTreePanel->IsVisible();
}
