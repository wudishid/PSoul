// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulGameMode_Game.h"
#include "GameFramework/Game/SoulGameState_Game.h"

void ASoulGameMode_Game::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulGameMode_Game::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ASoulGameState_Game* GameState_Game = GetGameState<ASoulGameState_Game>())
	{
		if (!GameState_Game->IsGameStarted())
		{
			GameState_Game->StartGame();
		}
	}
}
