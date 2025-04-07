// Copyright Epic Games, Inc. All Rights Reserved.
#include "GameFramework/Game/PlayerCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Game/SoulPlayerController_Game.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Input/SoulInputComponent.h"
#include "PSoul/SoulGameplayTags.h"
#include "../../../../../../UE5.4.4/UnrealEngine-release/Engine/Plugins/Animation/MotionWarping/Source/MotionWarping/Public/MotionWarpingComponent.h"



//////////////////////////////////////////////////////////////////////////
// APSoulCharacter

APlayerCharacterBase::APlayerCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
}

void APlayerCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (ASoulPlayerController_Game* PC = GetPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);


			USoulInputComponent* SoulIC = Cast<USoulInputComponent>(PlayerInputComponent);
			if (ensureMsgf(
				SoulIC,
				TEXT(
					"Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to USoulInputComponent or a subclass of it."
				)))
			{
				// Add the key mappings that may have been set by the player
				SoulIC->AddInputMappings(PC->InputConfig, Subsystem);

				// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
				// be triggered directly by these input actions Triggered events. 
				TArray<uint32> BindHandles;
				SoulIC->BindAbilityActions(PC->InputConfig, this, &ThisClass::Input_AbilityInputTagPressed,
				                           &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

				SoulIC->BindNativeAction(PC->InputConfig, SoulGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this,
				                         &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				SoulIC->BindNativeAction(PC->InputConfig, SoulGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this,
				                         &ThisClass::Input_Look, /*bLogIfNotFound=*/ false);
			}
		}
	}
}

void APlayerCharacterBase::Input_Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacterBase::Input_Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacterBase::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void APlayerCharacterBase::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void APlayerCharacterBase::HandleKill()
{
	Super::HandleKill();
	
	if (ASoulPlayerController_Game* PC = GetPlayerController())
	{
		PC->HandlePlayerKill();
	}
}

void APlayerCharacterBase::HandleDeath()
{
	Super::HandleDeath();

	GetCharacterMovement()->StopMovementImmediately();
}

void APlayerCharacterBase::FinishDeath()
{
	Super::FinishDeath();

	if (ASoulPlayerController_Game* PC = GetPlayerController())
	{
		PC->HandlePlayerDeath();
	}
}
