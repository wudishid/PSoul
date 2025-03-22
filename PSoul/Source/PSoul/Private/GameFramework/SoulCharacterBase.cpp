// Fill out your copyright notice in the Description page of Project Settings.
#include "PSoul/Public/GameFramework/SoulCharacterBase.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/SoulCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
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
	AttributeComponent->OnCharacterDeath.AddDynamic(this, &ThisClass::HandleDeath);
	
	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarComp->SetupAttachment(GetMesh());
	
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

void ASoulCharacterBase::NetMulticastHandleDeath_Implementation()
{
	HealthBarComp->SetHiddenInGame(true);
}

void ASoulCharacterBase::FinishDeath()
{
	
}

void ASoulCharacterBase::HandleDeath()
{
	NetMulticastHandleDeath();
}



