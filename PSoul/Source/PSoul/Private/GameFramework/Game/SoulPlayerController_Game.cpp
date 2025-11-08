// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulPlayerController_Game.h"
#include "EnhancedInputSubsystems.h"
#include "Components/QuickSkillManager.h"
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

ASoulPlayerController_Game::ASoulPlayerController_Game(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	QuickSkillManager = CreateDefaultSubobject<UQuickSkillManager>(TEXT("QuickSkillManager"));
}

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

		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill1, ETriggerEvent::Completed, this,
								 &ThisClass::PressSkill1, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill2, ETriggerEvent::Completed, this,
								 &ThisClass::PressSkill2, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill3, ETriggerEvent::Completed, this,
								 &ThisClass::PressSkill3, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill4, ETriggerEvent::Completed, this,
								 &ThisClass::PressSkill4, /*bLogIfNotFound=*/ false);
		
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

	QuickSkillManager->OnSetPawn(P);
}

void ASoulPlayerController_Game::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	QuickSkillManager->OnSetPawn(InPawn);
}

void ASoulPlayerController_Game::OnUnPossess()
{
	Super::OnUnPossess();
	if (QuickSkillManager)
	{
		QuickSkillManager->OnSetPawn(nullptr);
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

void ASoulPlayerController_Game::PressSkill1()
{
	QuickSkillManager->PressSkill(SoulGameplayTags::InputTag_QuickSkill1);
}

void ASoulPlayerController_Game::PressSkill2()
{
	QuickSkillManager->PressSkill(SoulGameplayTags::InputTag_QuickSkill2);
}

void ASoulPlayerController_Game::PressSkill3()
{
	QuickSkillManager->PressSkill(SoulGameplayTags::InputTag_QuickSkill3);
}

void ASoulPlayerController_Game::PressSkill4()
{
	QuickSkillManager->PressSkill(SoulGameplayTags::InputTag_QuickSkill4);
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

void ASoulPlayerController_Game::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulPlayerController_Game::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

