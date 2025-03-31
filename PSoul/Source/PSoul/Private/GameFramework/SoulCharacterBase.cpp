// Fill out your copyright notice in the Description page of Project Settings.
#include "PSoul/Public/GameFramework/SoulCharacterBase.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/DamageCheckComponent.h"
#include "Components/SoulCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Equipment/Equipment_Weapon.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Character/StateBar.h"


// Sets default values
ASoulCharacterBase::ASoulCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USoulCharacterMovementComponent>(CharacterMovementComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("SoulAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->SetIsReplicated(true);
	AttributeComponent->OnCharacterDeath.AddDynamic(this, &ThisClass::HandleDeath);
	
	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarComp->SetupAttachment(GetMesh());

	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManagerComp"));
	InventoryManagerComponent->SetIsReplicated(true);

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManagerComp"));
	EquipmentManagerComponent->SetIsReplicated(true);
	EquipmentManagerComponent->OnEquip.AddUObject(this, &ThisClass::HandleEquip);
	EquipmentManagerComponent->OnUnEquip.AddUObject(this, &ThisClass::HandleUnEquip);
	
	DamageCheckComponent = CreateDefaultSubobject<UDamageCheckComponent>(TEXT("DamageCheckComponent"));
	DamageCheckComponent->SetIsReplicated(true);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ASoulCharacterBase::HandleKill_Implementation()
{
	
}

// Called when the game starts or when spawned
void ASoulCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeComponent->InitWithAbilitySystemComponent(AbilitySystemComponent);

	if (!HasAuthority() && !IsLocallyControlled())
	{
		HealthBarComp->SetHiddenInGame(false);
		if (UStateBar* StateBar = Cast<UStateBar>(HealthBarComp->GetUserWidgetObject()))
		{
			StateBar->Init(this, USoulCharacterSet::GetHealthAttribute(), USoulCharacterSet::GetMaxHealthAttribute());
		}
	}
	else
	{
		HealthBarComp->SetHiddenInGame(true);
	}
}

void ASoulCharacterBase::NotifyRestarted()
{
	Super::NotifyRestarted();
}

void ASoulCharacterBase::HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		if(AEquipment_Weapon* Weapon = Cast<AEquipment_Weapon>(EquipmentInstance))
		{
			DamageCheckComponent->SetCheckByMesh(Weapon->GetWeaponMesh());
		}
	}
}

void ASoulCharacterBase::HandleUnEquip(EEquipmentType InEquipmentType)
{
	if(InEquipmentType == EEquipmentType::Weapon)
	{
		DamageCheckComponent->SetCheckByBoxTrace();
	}
}

void ASoulCharacterBase::FinishDeath()
{
	
}

void ASoulCharacterBase::HandleDeath()
{
	if (!HasAuthority() && !IsLocallyControlled())
	{
		HealthBarComp->SetHiddenInGame(true);
	}
}



