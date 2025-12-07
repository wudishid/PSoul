// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SoulGameTipWindow.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API USoulGameTipWindow : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FString& InMessage);

protected:
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Tip;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Start;
};
