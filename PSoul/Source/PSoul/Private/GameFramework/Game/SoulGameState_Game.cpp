// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulGameState_Game.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Net/UnrealNetwork.h"

ASoulGameState_Game::ASoulGameState_Game()
{
}

void ASoulGameState_Game::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulGameState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}






