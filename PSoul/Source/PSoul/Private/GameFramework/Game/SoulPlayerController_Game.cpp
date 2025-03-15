// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Game/SoulPlayerController_Game.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Game/SoulHUD_Game.h"
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

void ASoulPlayerController_Game::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	if(ASoulHUD_Game* HUD = Cast<ASoulHUD_Game>(GetHUD()))
	{
		HUD->InitHUD();
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

void ASoulPlayerController_Game::RestartPlayer()
{
	APawn* CurPawn = GetPawn();
	CurPawn->DetachFromControllerPendingDestroy();
	CurPawn->Destroy();
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}
