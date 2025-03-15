// Fill out your copyright notice in the Description page of Project Settings.


#include "PSoul/Public/GameFramework/SoulCharacterBase.h"
#include "Components/CharacterAttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "UI/Character/StateBar.h"


// Sets default values
ASoulCharacterBase::ASoulCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("SoulAbilitySystemComponent"));
	AttributeComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("AttributeComponent"));

	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarComp->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void ASoulCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() && !IsLocallyControlled())
	{
		HealthBarComp->SetHiddenInGame(false);
		if (UStateBar* StateBar = Cast<UStateBar>(HealthBarComp->GetUserWidgetObject()))
		{
			StateBar->Init(this, "Health", "MaxHealth");
		}
	}
	else
	{
		HealthBarComp->SetHiddenInGame(true);
	}
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeComponent->InitWithAbilitySystemComponent(AbilitySystemComponent);
	
}

