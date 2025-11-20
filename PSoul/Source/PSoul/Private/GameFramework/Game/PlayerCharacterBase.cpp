// Copyright Epic Games, Inc. All Rights Reserved.
#include "GameFramework/Game/PlayerCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Game/SoulPlayerController_Game.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Camera/SoulCameraComponent.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/DamageCheckComponent.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Equipment/Equipment_Weapon.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "GAS/GameplayEffect/SoulGameplayEffect.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SkillTreeSystem/SkillTreeManager.h"


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
	Camera = CreateDefaultSubobject<USoulCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManagerComp"));
	InventoryManagerComponent->SetIsReplicated(true);

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManagerComp"));
	EquipmentManagerComponent->SetIsReplicated(true);
	EquipmentManagerComponent->OnEquip.AddUObject(this, &ThisClass::HandleEquip);
	EquipmentManagerComponent->OnUnEquip.AddUObject(this, &ThisClass::HandleUnEquip);
	
	SkillTreeManagerComp = CreateDefaultSubobject<USkillTreeManager>(TEXT("SkillTreeManagerComp"));
	SkillTreeManagerComp->SetIsReplicated(true);
}

FRotator APlayerCharacterBase::GetDesiredRotation() const
{
	FVector LastInputVector = GetLastMovementInputVector();
	if (!LastInputVector.IsZero())
	{
		return LastInputVector.Rotation();
	}
	return Super::GetDesiredRotation();
}

void APlayerCharacterBase::HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		if(AEquipment_Weapon* Weapon = Cast<AEquipment_Weapon>(EquipmentInstance))
		{
			DamageCheckComponent->SetCheckByMesh(Weapon->GetWeaponMesh());
		}
	}
}

void APlayerCharacterBase::HandleUnEquip(EEquipmentType InEquipmentType)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		DamageCheckComponent->SetCheckByBoxTrace();
	}
}

void APlayerCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}


void APlayerCharacterBase::HandleKill(AActor* InKilled)
{
	Super::HandleKill(InKilled);

	if (UCharacterAttributeComponent* KilledAttributeComponent = InKilled->FindComponentByClass<UCharacterAttributeComponent>())
	{
		//获取灵魂
		int32 Soul = KilledAttributeComponent->GetAttributeValue(USoulCharacterSet::GetSoulAttribute());
		GetAbilitySystemComponent()->ApplyModToAttribute(USoulPlayerSet::GetSoulAttribute(), EGameplayModOp::Additive, Soul);
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

	if (ASoulPlayerController_Game* PC = Cast<ASoulPlayerController_Game>(GetController()))
	{
		PC->HandlePlayerDeath();
	}
}

