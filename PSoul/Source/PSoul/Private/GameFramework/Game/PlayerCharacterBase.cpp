// Copyright Epic Games, Inc. All Rights Reserved.
#include "GameFramework/Game/PlayerCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/SoulCameraComponent.h"
#include "Components/DamageCheckComponent.h"
#include "Components/WidgetComponent.h"
#include "Development/Soul_CommonSetting.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Equipment/Equipment_Weapon.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Misc/SoulGameFunctionLibrary.h"
#include "Save/SoulSaveGame_PlayerData.h"
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
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = false;
	
	// Create a follow camera
	Camera = CreateDefaultSubobject<USoulCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false;
	
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManagerComp"));
	InventoryManagerComponent->SetIsReplicated(true);

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManagerComp"));
	EquipmentManagerComponent->SetIsReplicated(true);
	EquipmentManagerComponent->OnEquip.AddUObject(this, &ThisClass::HandleEquip);
	EquipmentManagerComponent->OnUnEquip.AddUObject(this, &ThisClass::HandleUnEquip);
	
	SkillTreeManagerComp = CreateDefaultSubobject<USkillTreeManager>(TEXT("SkillTreeManagerComp"));
	SkillTreeManagerComp->SetIsReplicated(true);
	
	SkillDirectionSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SkillDirectionSceneComp"));
	SkillDirectionSceneComp->SetIsReplicated(true);
	SkillDirectionSceneComp->SetupAttachment(RootComponent);
	
	SkillDirectionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("SkillDirectionWidget"));
	SkillDirectionWidgetComp->SetupAttachment(SkillDirectionSceneComp);
	SkillDirectionWidgetComp->SetHiddenInGame(true);
}

FRotator APlayerCharacterBase::GetDesiredRotation() const
{
	return Super::GetDesiredRotation();
}

void APlayerCharacterBase::PickUpItem(FName InItemName, int32 Amount)
{
	InventoryManagerComponent->AddItem(InItemName, Amount);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->ClientPlaySoundAtLocation(GetDefault<USoul_CommonSetting>()->PickItemSound.LoadSynchronous(), GetActorLocation());
	}
}

void APlayerCharacterBase::SetEnableDirectionalSkillControl(bool InEnable)
{
	bEnableDirectionalSkillControl = InEnable;
	SkillDirectionSceneComp->SetHiddenInGame(!bEnableDirectionalSkillControl, true);
	if (InEnable)
	{
		GetController()->SetControlRotation(GetActorRotation());
	}
}

void APlayerCharacterBase::ApplyDirectionalSkillControl_Implementation()
{
	SetActorRotation(FRotator(GetActorRotation().Pitch, GetControlRotation().Yaw, GetActorRotation().Roll));
}

void APlayerCharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//玩家生成不同位置时保持摄像机臂的相对旋转
	CameraBoom->SetRelativeRotation(FRotator(CameraBoom->GetRelativeRotation().Pitch, Transform.Rotator().Yaw,
											 CameraBoom->GetRelativeRotation().Roll));
}

void APlayerCharacterBase::HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		if(AEquipment_Weapon* Weapon = Cast<AEquipment_Weapon>(EquipmentInstance))
		{
			DamageCheckComp->SetCheckByMesh(Weapon->GetWeaponMesh());
		}
	}
}

void APlayerCharacterBase::HandleUnEquip(EEquipmentType InEquipmentType)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		DamageCheckComp->SetCheckByBoxTrace();
	}
}

void APlayerCharacterBase::UpdateDirectionalSkillControl()
{
	if (IsLocallyControlled())
	{
		if (bEnableDirectionalSkillControl)
		{
			FRotator TargetRotation = SkillDirectionSceneComp->GetComponentRotation();
			TargetRotation.Yaw = GetControlRotation().Yaw;
			SkillDirectionSceneComp->SetWorldRotation(TargetRotation);
		}
	}
}

void APlayerCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	SetEnableDirectionalSkillControl(false);
	if (GetNetMode() == NM_Standalone)
	{
		HideHealthBar();
	}
	else
	{
		if (!IsLocallyControlled())
		{
			ShowHealthBar();
		}
		else
		{
			HideHealthBar();
		}
	}

	FTimerHandle Timer_Save;
	GetWorld()->GetTimerManager().SetTimer(Timer_Save, FTimerDelegate::CreateLambda([this]()
	{
		if (HasAuthority() || GetNetMode() == NM_Standalone)
		{
			if (USoulSaveGame_PlayerData* SaveGame_PlayerData = USoulGameFunctionLibrary::LoadGame())
			{
				if (USoulCharacterSet* CharacterSet = const_cast<USoulCharacterSet*>(ASC->GetSet<USoulCharacterSet>()))
				{
					CharacterSet->SetHealth(SaveGame_PlayerData->SavedData.CharacterData.Health.GetCurrentValue());
					CharacterSet->SetStamina(SaveGame_PlayerData->SavedData.CharacterData.Stamina.GetCurrentValue());
				}
			
				if (USoulPlayerSet* PlayerSet = const_cast<USoulPlayerSet*>(ASC->GetSet<USoulPlayerSet>()))
				{
					PlayerSet->SetLevel(SaveGame_PlayerData->SavedData.PlayerData.Level.GetCurrentValue());
					PlayerSet->SetNextLevelNeedSoul(
						SaveGame_PlayerData->SavedData.PlayerData.NextLevelNeedSoul.GetCurrentValue());
					PlayerSet->SetLife(SaveGame_PlayerData->SavedData.PlayerData.Life.GetCurrentValue());
					PlayerSet->SetStrength(SaveGame_PlayerData->SavedData.PlayerData.Strength.GetCurrentValue());
					PlayerSet->SetStamina(SaveGame_PlayerData->SavedData.PlayerData.Stamina.GetCurrentValue());
					PlayerSet->SetSoul(SaveGame_PlayerData->SavedData.PlayerData.Soul.GetCurrentValue());
				}

				for (const FSavedSingleItemData& SavedSingleItemData : SaveGame_PlayerData->SavedData.
						 InventoryItemsData)
				{
					InventoryManagerComponent->AddItem(SavedSingleItemData.ItemName, SavedSingleItemData.ItemAmount);
				}

				for (const FName& ItemName : SaveGame_PlayerData->SavedData.WornItemData)
				{
					EquipmentManagerComponent->Equip(ItemName);
				}

				for (const FName& SkillName : SaveGame_PlayerData->SavedData.UnlockedSkills)
				{
					SkillTreeManagerComp->UnlockSkill(SkillName);
				}

				for (const FName& SkillName : SaveGame_PlayerData->SavedData.LearnedSkills)
				{
					SkillTreeManagerComp->TryLearnSkill(SkillName);
				}
			
			}
		}
	}
	), 1.f, false);
	
}

void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateDirectionalSkillControl();
}

void APlayerCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

