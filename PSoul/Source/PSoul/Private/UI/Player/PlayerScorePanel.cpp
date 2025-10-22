// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerScorePanel.h"

#include "Components/TextBlock.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Kismet/KismetTextLibrary.h"

void UPlayerScorePanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerScorePanel::HandleTeamChanged(ESoulCharacterTeam InTeam)
{
}

void UPlayerScorePanel::HandleKillNumberChanged(int32 InKillNumber, ESoulCharacterTeam InTeam)
{
	Text_KillNumber->SetText(UKismetTextLibrary::Conv_IntToText(InKillNumber));
}

void UPlayerScorePanel::HandleDeathNumberChanged(int32 InDeathNumber, ESoulCharacterTeam InTeam)
{
	Text_DeathNumber->SetText(UKismetTextLibrary::Conv_IntToText(InDeathNumber));
}