// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Game/GameRoundTip.h"
#include "Components/TextBlock.h"

void UGameRoundTip::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	RemoveFromParent();
}

void UGameRoundTip::Init(int32 InRoundIndex)
{
	Text_RoundTip->SetText(FText::FromString(FString::FromInt(InRoundIndex)));
	PlayAnimation(Start);
	ShowInViewport();
}
