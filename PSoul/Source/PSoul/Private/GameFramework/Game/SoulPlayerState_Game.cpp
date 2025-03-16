// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Net/UnrealNetwork.h"

void ASoulPlayerState_Game::InitPlayerState(int32 InKillNumber, int32 InDeathNumber, ESoulCharacterTeam InTeam)
{
	KillNumber = InKillNumber;
	DeathNumber = InDeathNumber;
	Team = InTeam;
}

void ASoulPlayerState_Game::AddKillNumber()
{
	KillNumber++;
}

void ASoulPlayerState_Game::AddDeathNumber()
{
	DeathNumber++;
}

void ASoulPlayerState_Game::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ASoulPlayerState_Game, KillNumber, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ASoulPlayerState_Game, DeathNumber, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ASoulPlayerState_Game, Team, COND_None, REPNOTIFY_Always);
	
}

void ASoulPlayerState_Game::OnRep_KillNumber()
{
	OnKillNumberChanged.Broadcast(KillNumber, Team);
}

void ASoulPlayerState_Game::OnRep_DeathNumber()
{
	OnDeathNumberChanged.Broadcast(DeathNumber, Team);
}

void ASoulPlayerState_Game::OnRep_Team()
{
	OnTeamChanged.Broadcast(Team);
}
