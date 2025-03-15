// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CharacterAttributeComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "PSoul/SoulGameplayTags.h"


// Sets default values for this component's properties
UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCharacterAttributeComponent::HandleAttributeChanged(FGameplayAttribute Attribute, float CurrentValue,
	float OldValue)
{
	OnAttributeChanged.Broadcast(Attribute, CurrentValue, OldValue);
}

void UCharacterAttributeComponent::HandleCharacterDeath_Implementation()
{
	if (ASC)
	{
		FGameplayEventData Payload;
		Payload.EventTag = SoulGameplayTags::GameplayEvent_Death;
		ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
	OnCharacterDeath.Broadcast();
}

void UCharacterAttributeComponent::InitWithAbilitySystemComponent(USoulAbilitySystemComponent* InASC)
{
	check(InASC);
	ASC = InASC;
	CharacterSet =  ASC->GetSet<USoulCharacterSet>();
	if(CharacterSet)
	{
		CharacterSet->OnSoulAttributeChanged.AddUObject(this, &ThisClass::HandleAttributeChanged);
		CharacterSet->OnCharacterDeath.AddUObject(this, &ThisClass::HandleCharacterDeath);
	}
}

float UCharacterAttributeComponent::GetAttributeValue(FGameplayAttribute Attribute) const
{
	if(CharacterSet)
	{
		return CharacterSet->GetAttributeValue(Attribute);
	}
	
	return 0.f;
}

