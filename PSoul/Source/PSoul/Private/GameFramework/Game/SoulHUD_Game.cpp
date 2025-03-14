// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Game/SoulHUD_Game.h"
#include "UI/Player/PlayerPanel.h"

void ASoulHUD_Game::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(PlayerPanelClass))
	{
		if(UPlayerPanel* PlayerPanel = CreateWidget<UPlayerPanel>(GetOwningPlayerController(), PlayerPanelClass))
		{
			PlayerPanel->AddToViewport();
		}
	}
}
