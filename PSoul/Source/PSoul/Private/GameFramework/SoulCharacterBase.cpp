// Fill out your copyright notice in the Description page of Project Settings.
#include "PSoul/Public/GameFramework/SoulCharacterBase.h"

#include <filesystem>

#include "MotionWarpingComponent.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/DamageCheckComponent.h"
#include "Components/SoulCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "PSoul/SoulGameplayTags.h"
#include "UI/Character/StateBar.h"


// Sets default values
ASoulCharacterBase::ASoulCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USoulCharacterMovementComponent>(CharacterMovementComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("SoulAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
	
	AttributeComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->SetIsReplicated(true);
	AttributeComponent->OnAttributeChanged.AddDynamic(this, &ThisClass::HandleAttributeChanged);
	
	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarComp->SetupAttachment(GetMesh());
	
	DamageCheckComp = CreateDefaultSubobject<UDamageCheckComponent>(TEXT("DamageCheckComp"));
	DamageCheckComp->SetIsReplicated(true);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ASoulCharacterBase::HandleKill_Implementation(AActor* InCauser)
{
	if (USoulAbilitySystemComponent* CauserASC = InCauser->FindComponentByClass<USoulAbilitySystemComponent>())
	{
		if (CauserASC->GetSet<USoulPlayerSet>())
		{
			CauserASC->ApplyModToAttribute(USoulPlayerSet::GetSoulAttribute(), EGameplayModOp::Additive, AttributeComponent->GetAttributeValue(USoulCharacterSet::GetSoulAttribute()));
		}
	}
}


// Called when the game starts or when spawned
void ASoulCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->GetSet<USoulCharacterSet>()->OnDied.AddUObject(this, &ThisClass::HandleKill);
	
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

void ASoulCharacterBase::HandleAttributeChanged(FGameplayAttribute Attribute, float CurrentValue, float OldValue)
{
	if (Attribute == USoulCharacterSet::GetHealthAttribute())
	{
		if (CurrentValue <= 0)
		{
			OnDeath();
		}
	}
}

void ASoulCharacterBase::OnDeath()
{
	if (HasAuthority())
	{
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(SoulGameplayTags::Ability_Action_Died));
	}
	
	if (!HasAuthority() && !IsLocallyControlled())
	{
		HealthBarComp->SetHiddenInGame(true);
	}
}

FRotator ASoulCharacterBase::GetDesiredRotation() const
{
	return GetActorRotation();
}

FGenericTeamId ASoulCharacterBase::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(CharacterTeam));
}




