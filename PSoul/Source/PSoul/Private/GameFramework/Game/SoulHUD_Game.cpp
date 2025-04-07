// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Game/SoulHUD_Game.h"
#include "UI/Player/PlayerPanel.h"

void ASoulHUD_Game::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulHUD_Game::InitHUD()
{
	if(PlayerPanel)
	{
		PlayerPanel->InitPanel();
	}
	else
	{
		if(IsValid(PlayerPanelClass))
		{
			PlayerPanel = CreateWidget<UPlayerPanel>(GetOwningPlayerController(), PlayerPanelClass);
			PlayerPanel->AddToViewport();
			PlayerPanel->InitPanel();
		}
	}
}

void ASoulHUD_Game::SetShowInventoryPanel(bool bShow)
{
	if(PlayerPanel)
	{
		PlayerPanel->SetShowInventoryPanel(bShow);
	}
}

bool ASoulHUD_Game::IsShowingInventoryPanel() const
{
	if (PlayerPanel)
	{
		return PlayerPanel->IsShowInventoryPanel();
	}
	return false;
}


