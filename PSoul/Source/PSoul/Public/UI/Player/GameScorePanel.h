// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "GameScorePanel.generated.h"


class UTextBlock;

UCLASS()
class PSOUL_API UGameScorePanel : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void HandleRedPlayersScoreChanged(int32 InScore);
	void HandleBluePlayersScoreChanged(int32 InScore);
	void HandleGameTimeLengthChanged(float InRemainTime);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_RedPlayersScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_BluePlayersScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_TimeRemain;
};
