// Fill out your copyright notice in the Description page of Project Settings.


#include "PSoul/Public/GameFramework/SoulCharacterBase.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "UI/Character/StateBar.h"


// Sets default values
ASoulCharacterBase::ASoulCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("SoulAbilitySystemComponent"));
	
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

	if(IsLocallyControlled())
	{
		
	}
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

void ASoulCharacterBase::FinishDeath()
{
	
}

void ASoulCharacterBase::HandleDeath()
{
	
}



