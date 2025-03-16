// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "PlayerScorePanel.generated.h"

enum class ESoulCharacterTeam : uint8;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PSOUL_API UPlayerScorePanel : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void HandleTeamChanged(ESoulCharacterTeam InTeam);
	void HandleKillNumberChanged(int32 InKillNumber, ESoulCharacterTeam InTeam);
	void HandleDeathNumberChanged(int32 InDeathNumber, ESoulCharacterTeam InTeam);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Team;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_KillNumber;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_DeathNumber;
	
};
