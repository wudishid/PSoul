// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CharacterAttributeComponent.h"

#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "PSoul/SoulGameplayTags.h"



UCharacterAttributeComponent::UCharacterAttributeComponent()
{
}

void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterAttributeComponent::HandleAttributeChanged(FGameplayAttribute Attribute, float CurrentValue,
	float OldValue)
{
	OnAttributeChanged.Broadcast(Attribute, CurrentValue, OldValue);
}

void UCharacterAttributeComponent::HandleCharacterDeath(AActor* InCauser)
{
	Client_SendDeathEvent();
	OnCharacterDeath.Broadcast();
	if(ASoulCharacterBase* CauserCharacter = Cast<ASoulCharacterBase>(InCauser))
	{
		CauserCharacter->HandleKill();
	}
}

void UCharacterAttributeComponent::Client_SendDeathEvent_Implementation()
{
	if (ASC)
	{
		FGameplayEventData Payload;
		Payload.EventTag = SoulGameplayTags::GameplayEvent_Death;
		ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
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

