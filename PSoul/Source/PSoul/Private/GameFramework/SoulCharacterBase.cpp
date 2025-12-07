// Fill out your copyright notice in the Description page of Project Settings.
#include "PSoul/Public/GameFramework/SoulCharacterBase.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/DamageCheckComponent.h"
#include "Components/SoulCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "PSoul/SoulGameplayTags.h"
#include "UI/Character/StateBar.h"
#include "UI/Character/StateBar_Info.h"


// Sets default values
ASoulCharacterBase::ASoulCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USoulCharacterMovementComponent>(CharacterMovementComponentName))
{
	ASC = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("SoulAbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
	
	AttributeComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->SetIsReplicated(true);
	AttributeComponent->OnAttributeChanged.AddDynamic(this, &ThisClass::HandleAttributeChanged);
	
	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarComp->SetupAttachment(GetMesh());
	HealthBarComp->SetCollisionProfileName(TEXT("SoulUI"));
	
	DamageCheckComp = CreateDefaultSubobject<UDamageCheckComponent>(TEXT("DamageCheckComp"));
	DamageCheckComp->SetIsReplicated(true);
	
	GetMesh()->SetCollisionProfileName(TEXT("SoulCharacterMesh"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SoulCharacterCapsule"));
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

	if (HasAuthority())
	{
		ASC->InitAbilityActorInfo(this, this);
	}
	
	ASC->GetSet<USoulCharacterSet>()->OnDied.AddUObject(this, &ThisClass::HandleKill);
	
	AttributeComponent->InitWithAbilitySystemComponent(ASC);
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

void ASoulCharacterBase::ShowHealthBar()
{
	HealthBarComp->SetHiddenInGame(false);
	if (UStateBar_Info* StateBar_Info = Cast<UStateBar_Info>(HealthBarComp->GetUserWidgetObject()))
	{
		StateBar_Info->Init(this, USoulCharacterSet::GetHealthAttribute(), USoulCharacterSet::GetMaxHealthAttribute());
	}
}

void ASoulCharacterBase::HideHealthBar()
{
	HealthBarComp->SetHiddenInGame(true);
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

	//死亡后，不对其他角色造成阻挡
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	OnCharacterDied.Broadcast(this);
}

void ASoulCharacterBase::FinishDeath()
{
	
}

FRotator ASoulCharacterBase::GetDesiredRotation() const
{
	return GetActorRotation();
}

FGenericTeamId ASoulCharacterBase::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(CharacterTeam));
}





