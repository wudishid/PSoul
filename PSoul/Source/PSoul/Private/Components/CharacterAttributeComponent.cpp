// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/CharacterAttributeComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"



UCharacterAttributeComponent::UCharacterAttributeComponent()
{
}

void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterAttributeComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UCharacterAttributeComponent::HandleAttributeChanged(FGameplayAttribute Attribute, float CurrentValue,
                                                          float OldValue)
{
	OnAttributeChanged.Broadcast(Attribute, CurrentValue, OldValue);
}

void UCharacterAttributeComponent::InitWithAbilitySystemComponent(USoulAbilitySystemComponent* InASC)
{
	check(InASC);
	ASC = InASC;
	CharacterSet =  ASC->GetSet<USoulCharacterSet>();
	if(CharacterSet)
	{
		CharacterSet->OnSoulAttributeChanged.AddUObject(this, &ThisClass::HandleAttributeChanged);
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

