// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/AICharacterBase.h"
#include "Data/AICharacterDataAsset.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Util/Util_Common.h"


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
	if (AICharacterData)
	{
		Util_Common::SpawnInventroyItemInstance(this, AICharacterData->DropedItem);
	}
}


