// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerPanel.h"

#include "Components/CharacterAttributeComponent.h"
#include "UI/Character/StateBar.h"

void UPlayerPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerPanel::InitPanel()
{
	HealthBar->Init(GetOwningPlayerPawn());
	StaminaBar->Init(GetOwningPlayerPawn());
}