// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulPlayerState_Game.h"

void ASoulPlayerState_Game::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASoulPlayerState_Game::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	OnPlayerNameChanged.Broadcast(GetPlayerName());
}


