// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulGameState_Game.h"
#include "Components/SoulGameManager.h"
#include "GameFramework/Game/SoulPlayerController_Game.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Net/UnrealNetwork.h"

ASoulGameState_Game::ASoulGameState_Game()
{
	GameManagerComp = CreateDefaultSubobject<USoulGameManager>(TEXT("GameManagerComp"));
	GameManagerComp->SetIsReplicated(true);
	GameManagerComp->OnGetQuerier.BindUObject(this, &ThisClass::OnGetQuerier);
	GameManagerComp->OnRoundStart.AddUObject(this, &ThisClass::OnRoundStart);
	GameManagerComp->OnGameOver.AddUObject(this, &ThisClass::OnGameOver);
}

void ASoulGameState_Game::StartGame()
{
	GameManagerComp->StartSpawnAI();
}

AActor* ASoulGameState_Game::OnGetQuerier()
{
	if (PlayerArray.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PlayerArray.Num() - 1);
		return PlayerArray[RandomIndex]->GetPawn();
	}
	
	return nullptr;
}

void ASoulGameState_Game::OnRoundStart(int32 RoundNum)
{
	for (auto& PlayerState : PlayerArray)
	{
		if (ASoulPlayerController_Game* PC = Cast<ASoulPlayerController_Game>(PlayerState->GetPlayerController()))
		{
			PC->ClientShowRoundTip(RoundNum);
		}
	}
}

void ASoulGameState_Game::OnGameOver()
{
	for (auto& PlayerState : PlayerArray)
	{
		if (ASoulPlayerController_Game* PC = Cast<ASoulPlayerController_Game>(PlayerState->GetPlayerController()))
		{
			PC->ClientShowGameOverTip();
		}
	}
}






