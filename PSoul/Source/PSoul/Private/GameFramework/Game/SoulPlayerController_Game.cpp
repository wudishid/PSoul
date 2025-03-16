// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Game/SoulPlayerController_Game.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Game/SoulHUD_Game.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "PSoul/SoulLog.h"

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

void ASoulPlayerController_Game::HandlePlayerDeath()
{
	if(ASoulPlayerState_Game* PS = GetPlayerState<ASoulPlayerState_Game>())
	{
		PS->AddDeathNumber();
	}
	
	APawn* CurPawn = GetPawn();
	CurPawn->DetachFromControllerPendingDestroy();
	CurPawn->Destroy();
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void ASoulPlayerController_Game::HandlePlayerKill()
{
	if(ASoulPlayerState_Game* PS = GetPlayerState<ASoulPlayerState_Game>())
	{
		PS->AddKillNumber();
	}
}

void ASoulPlayerController_Game::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulPlayerController_Game::InitPlayerState()
{
	Super::InitPlayerState();
	UE_LOG(LogSoul, Error, TEXT("InitPlayerState on %d"), GetWorld()->GetNetMode());
#if WITH_SERVER_CODE
	if(ASoulPlayerState_Game* PS = GetPlayerState<ASoulPlayerState_Game>())
	{
		ESoulCharacterTeam Team = FMath::RandBool() ? ESoulCharacterTeam::RedPlayer : ESoulCharacterTeam::BluePlayer;
		PS->InitPlayerState(1, 1, Team);
	}
#endif
	
}
