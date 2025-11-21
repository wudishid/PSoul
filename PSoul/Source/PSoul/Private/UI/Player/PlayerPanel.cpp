// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Player/PlayerPanel.h"
#include "Subsystem/UIPopupManager.h"
#include "UI/Character/StateBar.h"
#include "UI/Inventory/InventoryList.h"
#include "UI/Player/Attribute/AttributePanel.h"
#include "UI/SkillTree/SkillTreePanel.h"
#include "Util/Util_Common.h"


void UPlayerPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerPanel::InitPanel()
{
	HealthBar->Init(GetOwningPlayerPawn());
	StaminaBar->Init(GetOwningPlayerPawn());
}

void UPlayerPanel::SetShowInventoryPanel(bool bShow)
{
	if (bShow)
	{
		InventoryList->SetVisibility(ESlateVisibility::Visible);
		AttributePanel->SetVisibility(ESlateVisibility::Visible);
		Util_Common::PlayOpenPanelSound(GetWorld());
	}
	else
	{
		InventoryList->SetVisibility(ESlateVisibility::Hidden);
		AttributePanel->SetVisibility(ESlateVisibility::Hidden);
		Util_Common::PlayClosePanelSound(GetWorld());
	}
}

void UPlayerPanel::SetShowSkillTreePanel(bool bShow)
{
	if (bShow)
	{
		SkillTreePanel->SetVisibility(ESlateVisibility::Visible);
		Util_Common::PlayOpenPanelSound(GetWorld());
	}
	else
	{
		SkillTreePanel->SetVisibility(ESlateVisibility::Hidden);
		GetGameInstance()->GetSubsystem<UUIPopupManager>()->RemoveAllPopupWidgetsByLayer(EPopupWidgetLayer::SkillTree);
		Util_Common::PlayClosePanelSound(GetWorld());
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
