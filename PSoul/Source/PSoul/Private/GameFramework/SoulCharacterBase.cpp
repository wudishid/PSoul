// Fill out your copyright notice in the Description page of Project Settings.
#include "PSoul/Public/GameFramework/SoulCharacterBase.h"
#include "MotionWarpingComponent.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/DamageCheckComponent.h"
#include "Components/SoulCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
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
	AttributeComponent->OnCharacterDeath.AddDynamic(this, &ThisClass::HandleDeath);
	
	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarComp->SetupAttachment(GetMesh());
	
	DamageCheckComponent = CreateDefaultSubobject<UDamageCheckComponent>(TEXT("DamageCheckComponent"));
	DamageCheckComponent->SetIsReplicated(true);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ASoulCharacterBase::HandleKill_Implementation(AActor* InKilled)
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

void ASoulCharacterBase::FinishDeath()
{
	
}

FRotator ASoulCharacterBase::GetDesiredRotation() const
{
	return GetActorRotation();
}

void ASoulCharacterBase::HandleDeath()
{
	if (!HasAuthority() && !IsLocallyControlled())
	{
		HealthBarComp->SetHiddenInGame(true);
	}
}



