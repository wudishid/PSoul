// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulGameState_Game.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Net/UnrealNetwork.h"

ASoulGameState_Game::ASoulGameState_Game()
	: GameTimeLength(120), RedPlayersScore(0), BluePlayersScore(0)
{
}

void ASoulGameState_Game::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(Timer_GameTimeLength, [this]()
		{
			UpdatePlayerKillDeathNumber();
			
			GameTimeLength -= 1;
			FMath::Clamp(GameTimeLength, 0, 120);
			if(GameTimeLength <= 0)
			{
				GetWorld()->GetTimerManager().ClearTimer(Timer_GameTimeLength);
				GetWorld()->ServerTravel("ThirdPersonMap");
			}
			
		}, 1.f, true);
	}
}

void ASoulGameState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ASoulGameState_Game, GameTimeLength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ASoulGameState_Game, RedPlayersScore, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ASoulGameState_Game, BluePlayersScore, COND_None, REPNOTIFY_Always);
}

void ASoulGameState_Game::OnRep_GameTimeLength()
{
	OnGameTimeLengthChanged.Broadcast(GameTimeLength);
}

void ASoulGameState_Game::OnRep_RedPlayersScore()
{
	OnRedPlayersScoreChanged.Broadcast(RedPlayersScore);
}

void ASoulGameState_Game::OnRep_BluePlayersScore()
{
	OnBluePlayersScoreChanged.Broadcast(BluePlayersScore);
}

void ASoulGameState_Game::UpdatePlayerKillDeathNumber()
{
	if(!PlayerArray.IsEmpty())
	{
		int32 TempRedKillNumber = 0;
		int32 TempBlueKillNumber = 0;
		
		for(APlayerState* PS : PlayerArray)
		{
			if(ASoulPlayerState_Game* SoulPS = Cast<ASoulPlayerState_Game>(PS))
			{
				if(SoulPS->GetTeam() == ESoulCharacterTeam::RedPlayer)
				{
					TempRedKillNumber += SoulPS->GetKillNumber();
				}
				else if(SoulPS->GetTeam() == ESoulCharacterTeam::BluePlayer)
				{
					TempBlueKillNumber += SoulPS->GetKillNumber();
				}
			}
		}
		RedPlayersScore = TempRedKillNumber;
		BluePlayersScore = TempBlueKillNumber;
	}
}



