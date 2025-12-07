// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "GameRoundTip.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UGameRoundTip : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void Init(int32 InRoundIndex);

protected:
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_RoundTip;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Start;
};
