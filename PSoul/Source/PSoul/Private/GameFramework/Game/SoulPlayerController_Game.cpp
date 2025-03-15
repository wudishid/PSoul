// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Game/SoulPlayerController_Game.h"

#include "GameFramework/Character.h"
#include "GAS/SoulAbilitySystemComponent.h"

void ASoulPlayerController_Game::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ASoulPlayerController_Game::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	if(USoulAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
}

USoulAbilitySystemComponent* ASoulPlayerController_Game::GetAbilitySystemComponent() const
{
	static USoulAbilitySystemComponent* ASC = nullptr;
	if (!ASC && GetCharacter())
	{
		return GetCharacter()->FindComponentByClass<USoulAbilitySystemComponent>();
	}
	
	return ASC;
}
