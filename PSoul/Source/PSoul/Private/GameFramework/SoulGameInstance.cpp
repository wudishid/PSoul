// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SoulGameInstance.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"

void USoulGameInstance::Init()
{
	Super::Init();
	Team = FMath::RandBool() ? ESoulCharacterTeam::RedPlayer : ESoulCharacterTeam::BluePlayer;
	
	FParse::Value(FCommandLine::Get(), TEXT("-EnterGameMap="), EnterGameMap);
}
