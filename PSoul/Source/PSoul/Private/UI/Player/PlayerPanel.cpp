// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerPanel.h"

#include "Components/CharacterAttributeComponent.h"
#include "UI/Character/StateBar.h"

void UPlayerPanel::NativeConstruct()
{
	Super::NativeConstruct();
	
	HealthBar->Init(GetOwningPlayerPawn(), "Health", "MaxHealth");
	StaminaBar->Init(GetOwningPlayerPawn(), "Stamina", "MaxStamina");
}
