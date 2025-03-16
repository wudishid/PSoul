// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Game/SoulPlayerController_Game.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SoulGameInstance.h"
#include "GameFramework/Game/SoulHUD_Game.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
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

	FInputModeGameOnly ModeGameOnly;
	SetShowMouseCursor(false);
	SetInputMode(ModeGameOnly);
	
	if(ASoulHUD_Game* HUD = Cast<ASoulHUD_Game>(GetHUD()))
	{
		HUD->InitHUD();
	}
}

void ASoulPlayerController_Game::OnRep_Team()
{
	InitSoulPlayerState();
}

void ASoulPlayerController_Game::InitSoulPlayerState_Implementation()
{
	if(ASoulPlayerState_Game* PS = GetPlayerState<ASoulPlayerState_Game>())
	{
		PS->InitPlayerState(0, 0, Team);
	}
}

void ASoulPlayerController_Game::SetTeam_Implementation(ESoulCharacterTeam InTeam)
{
	Team = InTeam;
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

	if(IsLocalController())
	{
		USoulGameInstance* GameInstance = GetGameInstance<USoulGameInstance>();
		if(ensureMsgf(GameInstance, TEXT("SoulGameInstance Is not Valid!")))
		{
			SetTeam(GameInstance->Team);
		}
	}
}

void ASoulPlayerController_Game::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ASoulPlayerController_Game, Team, COND_None, REPNOTIFY_Always);
}

