// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Game/SoulGameTipWindow.h"
#include "Components/TextBlock.h"

void USoulGameTipWindow::Init(const FString& InMessage)
{
	Text_Tip->SetText(FText::FromString(InMessage));
	PlayAnimation(Start);
	ShowInViewport();
	
}

void USoulGameTipWindow::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	RemoveFromParent();
}
