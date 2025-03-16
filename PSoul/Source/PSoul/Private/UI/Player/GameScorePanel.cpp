// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/GameScorePanel.h"
#include "Components/TextBlock.h"
#include "GameFramework/Game/SoulGameState_Game.h"
#include "Kismet/KismetTextLibrary.h"

void UGameScorePanel::NativeConstruct()
{
	Super::NativeConstruct();

	if(ASoulGameState_Game* GS = GetWorld()->GetGameState<ASoulGameState_Game>())
	{
		HandleRedPlayersScoreChanged(GS->GetRedPlayersScore());
		HandleBluePlayersScoreChanged(GS->GetBluePlayersScore());
		
		GS->OnGameTimeLengthChanged.AddUObject(this, &ThisClass::HandleGameTimeLengthChanged);
		GS->OnRedPlayersScoreChanged.AddUObject(this, &ThisClass::HandleRedPlayersScoreChanged);
		GS->OnBluePlayersScoreChanged.AddUObject(this, &ThisClass::HandleBluePlayersScoreChanged);
	}
}

void UGameScorePanel::HandleRedPlayersScoreChanged(int32 InScore)
{
	Text_RedPlayersScore->SetText(UKismetTextLibrary::Conv_IntToText(InScore));
}

void UGameScorePanel::HandleBluePlayersScoreChanged(int32 InScore)
{
	Text_BluePlayersScore->SetText(UKismetTextLibrary::Conv_IntToText(InScore));
}

void UGameScorePanel::HandleGameTimeLengthChanged(float InRemainTime)
{
	int32 Minute = InRemainTime / 60;
	int32 Seconds = (int)InRemainTime % 60;
	Text_TimeRemain->SetText(FText::FromString(FString::Printf(TEXT("%02d : %02d"), Minute, Seconds)));
}


