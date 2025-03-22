// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AICharacterBase.h"
#include "GAS/SoulAbilitySystemComponent.h"


// Sets default values
AAICharacterBase::AAICharacterBase()
{
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacterBase::FinishDeath()
{
	Super::FinishDeath();
	Destroy();
}


