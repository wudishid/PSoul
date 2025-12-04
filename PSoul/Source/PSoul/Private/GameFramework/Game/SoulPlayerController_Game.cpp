// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Game/SoulPlayerController_Game.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/QuickSkillManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SoulGameInstance.h"
#include "GameFramework/Game/SoulHUD_Game.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Input/SoulInputComponent.h"
#include "Interface/SkillReleaseControlInterface.h"
#include "Misc/SoulGameFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PSoul/SoulGameplayTags.h"
#include "PSoul/SoulLog.h"
#include "Save/SoulSaveGame_PlayerData.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"

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
		// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
		// be triggered directly by these input actions Triggered events. 
		TArray<uint32> BindHandles;
		SoulIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed,
								   &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this,
								 &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this,
								 &ThisClass::Input_Look, /*bLogIfNotFound=*/ false);

		
		//背包面板
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_OpenInventoryPanel, ETriggerEvent::Completed, this,
		                         &ThisClass::ToggleShowInventoryPanel, /*bLogIfNotFound=*/ false);

		//技能树面板
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_OpenSkillTreePanel, ETriggerEvent::Completed, this,
								 &ThisClass::ToggleShowSkillTreePanel, /*bLogIfNotFound=*/ false);

		//技能快捷面板
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill1, ETriggerEvent::Started, this,
								 &ThisClass::PressSkill1, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill1, ETriggerEvent::Completed, this,
								 &ThisClass::ReleaseSkill1, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill2, ETriggerEvent::Started, this,
								 &ThisClass::PressSkill2, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill2, ETriggerEvent::Completed, this,
								 &ThisClass::ReleaseSkill2, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill3, ETriggerEvent::Started, this,
								 &ThisClass::PressSkill3, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill3, ETriggerEvent::Completed, this,
								 &ThisClass::ReleaseSkill3, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill4, ETriggerEvent::Started, this,
								 &ThisClass::PressSkill4, /*bLogIfNotFound=*/ false);
		SoulIC->BindNativeAction(InputConfig, SoulGameplayTags::InputTag_QuickSkill4, ETriggerEvent::Completed, this,
								 &ThisClass::ReleaseSkill4, /*bLogIfNotFound=*/ false);
		
	}
}

void ASoulPlayerController_Game::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	if(ASC.IsValid())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
}

void ASoulPlayerController_Game::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	ASC = P->FindComponentByClass<USoulAbilitySystemComponent>();
	QuickSkillManager->OnSetPawn(GetPawn());
	
	SetInputModeGame();
	if(ASoulHUD_Game* HUD = Cast<ASoulHUD_Game>(GetHUD()))
	{
		HUD->InitHUD();
	}

	// if (USoulSaveGame_PlayerData* SaveGame_PlayerData = USoulGameFunctionLibrary::LoadGame())
	// {
	// 	for (const auto& QuickSkillSlot : SaveGame_PlayerData->SavedData.QuickSkills)
	// 	{
	// 		QuickSkillManager->SetQuickSkill(QuickSkillSlot.Key, QuickSkillSlot.Value);
	// 	}
	// }
}

void ASoulPlayerController_Game::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ASC = InPawn->FindComponentByClass<USoulAbilitySystemComponent>();
	QuickSkillManager->OnSetPawn(GetPawn());
}

void ASoulPlayerController_Game::OnUnPossess()
{
	Super::OnUnPossess();
	if (QuickSkillManager)
	{
		QuickSkillManager->OnSetPawn(nullptr);
	}
}

void ASoulPlayerController_Game::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	SetPlayerName(GetGameInstance<USoulGameInstance>()->PlayerName);
}

void ASoulPlayerController_Game::Input_Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetPawn() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = PlayerCameraManager->GetCameraRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
		GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASoulPlayerController_Game::Input_Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetPawn() != nullptr)
	{
		// add yaw and pitch input to controller
		GetPawn()->AddControllerYawInput(LookAxisVector.X);
		GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASoulPlayerController_Game::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (ASC.IsValid())
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void ASoulPlayerController_Game::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (ASC.IsValid())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void ASoulPlayerController_Game::ToggleShowInventoryPanel()
{
	if(ASoulHUD_Game* HUD = Cast<ASoulHUD_Game>(GetHUD()))
	{
		if(HUD->IsShowingInventoryPanel())
		{
			HUD->SetShowInventoryPanel(false);
			SetInputModeGame();
		}
		else
		{
			HUD->SetShowInventoryPanel(true);
			SetInputModeUI(EInputMappingContextMode::Inventory);
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
			SetInputModeGame();
		}
		else
		{
			HUD->SetShowSkillTreePanel(true);
			SetInputModeUI(EInputMappingContextMode::SkillTree);
		}
	}
}

void ASoulPlayerController_Game::PressSkill1()
{
	PressSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill1);
}

void ASoulPlayerController_Game::ReleaseSkill1()
{
	ReleaseSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill1);
}

void ASoulPlayerController_Game::PressSkill2()
{
	PressSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill2);
}

void ASoulPlayerController_Game::ReleaseSkill2()
{
	ReleaseSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill2);
}

void ASoulPlayerController_Game::PressSkill3()
{
	PressSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill3);
}

void ASoulPlayerController_Game::ReleaseSkill3()
{
	ReleaseSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill3);
}

void ASoulPlayerController_Game::PressSkill4()
{
	PressSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill4);
}

void ASoulPlayerController_Game::ReleaseSkill4()
{
	ReleaseSkillByInputTag(SoulGameplayTags::InputTag_QuickSkill4);
}

void ASoulPlayerController_Game::PressSkillByInputTag(FGameplayTag InInputTag)
{
	if (!QuickSkillManager->CanReleaseSkill(InInputTag)) return;
	
	if (USkillTreeNodeData* SkillTreeNodeData =  QuickSkillManager->GetQuickSkillData(InInputTag))
	{
		if (SkillTreeNodeData->SkillReleaseType == ESkillReleaseType::Normal)
		{
			QuickSkillManager->ReleaseSkill(InInputTag);
		}
		else if (SkillTreeNodeData->SkillReleaseType == ESkillReleaseType::DirectionRelease)
		{
			if (ASC.IsValid())
			{
				ASC->AddLooseGameplayTag(SoulGameplayTags::Help_SkillReleaseDirection);
			}
		}
	}
}

void ASoulPlayerController_Game::ReleaseSkillByInputTag(FGameplayTag InInputTag)
{
	if (!QuickSkillManager->CanReleaseSkill(InInputTag)) return;
	
	if (USkillTreeNodeData* SkillTreeNodeData =  QuickSkillManager->GetQuickSkillData(InInputTag))
	{
		if (SkillTreeNodeData->SkillReleaseType == ESkillReleaseType::DirectionRelease)
		{
			QuickSkillManager->ReleaseSkill(InInputTag);

			if (ASC.IsValid())
			{
				ASC->RemoveLooseGameplayTag(SoulGameplayTags::Help_SkillReleaseDirection);
			}
		}
	}
}

void ASoulPlayerController_Game::SetInputMappingContextMode(EInputMappingContextMode InContextMode)
{
	UInputMappingContext* Context = *InputMappingContexts.Find(InContextMode);
	if (ensureMsgf(Context, TEXT("Could not find context, check context is setted..")))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(Context, 0);
		}
	}
}

void ASoulPlayerController_Game::SetInputModeGame()
{
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
	SetInputMappingContextMode(EInputMappingContextMode::Default);
}

void ASoulPlayerController_Game::SetInputModeUI(EInputMappingContextMode InContextMode)
{
	SetShowMouseCursor(true);
	SetInputMode(FInputModeGameAndUI());
	SetInputMappingContextMode(InContextMode);
}

void ASoulPlayerController_Game::SetPlayerName_Implementation(FName InName)
{
	if (PlayerState)
	{
		PlayerState->SetPlayerName(InName.ToString());
	}
}

void ASoulPlayerController_Game::Rebirth()
{
	if (APawn* CurPawn = GetPawn())
	{
		CurPawn->DetachFromControllerPendingDestroy();
		CurPawn->Destroy();
		GetWorld()->GetAuthGameMode()->RestartPlayer(this);
	}
}

void ASoulPlayerController_Game::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulPlayerController_Game::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

