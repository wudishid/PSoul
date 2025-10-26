// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulPlayerController_Game.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SoulGameInstance.h"
#include "GameFramework/Game/SoulHUD_Game.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Input/SoulInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "PSoul/SoulGameplayTags.h"
#include "PSoul/SoulLog.h"

void ASoulPlayerController_Game::SetupInputComponent()
{
	Super::SetupInputComponent();

	USoulInputComponent* SoulIC = Cast<USoulInputComponent>(InputComponent);
	if (ensureMsgf(
		SoulIC,
		TEXT(
			"Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to USoulInputComponent or a subclass of it."
		)))
	{
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_OpenInventoryPanel, ETriggerEvent::Completed, this,
		                         &ThisClass::ToggleShowInventoryPanel, /*bLogIfNotFound=*/ false);

		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_OpenSkillTreePanel, ETriggerEvent::Completed, this,
								 &ThisClass::ToggleShowSkillTreePanel, /*bLogIfNotFound=*/ false);
		
	}
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

void ASoulPlayerController_Game::ToggleShowInventoryPanel()
{
	if(ASoulHUD_Game* HUD = Cast<ASoulHUD_Game>(GetHUD()))
	{
		if(HUD->IsShowingInventoryPanel())
		{
			HUD->SetShowInventoryPanel(false);
			SetShowMouseCursor(false);
			SetIgnoreLookInput(false);
			SetInputMode(FInputModeGameOnly());
		}
		else
		{
			HUD->SetShowInventoryPanel(true);
			SetShowMouseCursor(true);
			SetIgnoreLookInput(true);
			SetInputMode(FInputModeGameAndUI());
		}
	}
}

void ASoulPlayerController_Game::ToggleShowSkillTreePanel()
{
	if(ASoulHUD_Game* HUD = Cast<ASoulHUD_Game>(GetHUD()))
	{
		if(HUD->IsShowingSkillTreePanel())
		{
			HUD->SetShowSkillTreePanel(false);
			SetShowMouseCursor(false);
			SetIgnoreLookInput(false);
			SetInputMode(FInputModeGameOnly());
		}
		else
		{
			HUD->SetShowSkillTreePanel(true);
			SetShowMouseCursor(true);
			SetIgnoreLookInput(true);
			SetInputMode(FInputModeGameAndUI());
		}
	}
}

void ASoulPlayerController_Game::InitSoulPlayerState_Implementation()
{
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
	APawn* CurPawn = GetPawn();
	CurPawn->DetachFromControllerPendingDestroy();
	CurPawn->Destroy();
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void ASoulPlayerController_Game::HandlePlayerKill()
{
	
}

void ASoulPlayerController_Game::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulPlayerController_Game::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

