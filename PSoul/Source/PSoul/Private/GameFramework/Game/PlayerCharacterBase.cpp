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
#include "Components/WidgetComponent.h"
#include "Development/Soul_CommonSetting.h"
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

void APlayerCharacterBase::PickUpItem(const FInventoryItemInfo& ItemInfo)
{
	InventoryManagerComponent->AddItem(ItemInfo);
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
}

void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateDirectionalSkillControl();
}

