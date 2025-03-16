// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerScorePanel.h"

#include "Components/TextBlock.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Kismet/KismetTextLibrary.h"

void UPlayerScorePanel::NativeConstruct()
{
	Super::NativeConstruct();

	if(ASoulPlayerState_Game* PS = GetOwningPlayerState<ASoulPlayerState_Game>())
	{
		HandleKillNumberChanged(PS->GetKillNumber(), PS->GetTeam());
		HandleDeathNumberChanged(PS->GetDeathNumber(), PS->GetTeam());
		HandleTeamChanged(PS->GetTeam());
		
		PS->OnTeamChanged.AddUObject(this, &ThisClass::HandleTeamChanged);
		PS->OnKillNumberChanged.AddUObject(this, &ThisClass::HandleKillNumberChanged);
		PS->OnDeathNumberChanged.AddUObject(this, &ThisClass::HandleDeathNumberChanged);
	}
}

void UPlayerScorePanel::HandleTeamChanged(ESoulCharacterTeam InTeam)
{
	switch (InTeam)
	{
	case ESoulCharacterTeam::RedPlayer:
		Text_Team->SetText(FText::FromName("Red"));
		Text_Team->SetColorAndOpacity(FColor::Red);
		break;
	case ESoulCharacterTeam::BluePlayer:
		Text_Team->SetText(FText::FromName("Blue"));
		Text_Team->SetColorAndOpacity(FColor::Blue);
		break;
	default:
		Text_Team->SetText(FText::FromName("NoneTeam"));
		Text_Team->SetColorAndOpacity(FColor::White);
		break;
	}
}

void UPlayerScorePanel::HandleKillNumberChanged(int32 InKillNumber, ESoulCharacterTeam InTeam)
{
	Text_KillNumber->SetText(UKismetTextLibrary::Conv_IntToText(InKillNumber));
}

void UPlayerScorePanel::HandleDeathNumberChanged(int32 InDeathNumber, ESoulCharacterTeam InTeam)
{
	Text_DeathNumber->SetText(UKismetTextLibrary::Conv_IntToText(InDeathNumber));
}