// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/SoulCharacterSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "PSoul/SoulGameplayTags.h"
#include "PSoul/SoulLog.h"

USoulCharacterSet::USoulCharacterSet()
:Health(100),
MaxHealth(100),
Stamina(50),
MaxStamina(50)
{
	
}

void USoulCharacterSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USoulCharacterSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulCharacterSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulCharacterSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulCharacterSet, MaxStamina, COND_None, REPNOTIFY_Always);
	
}

bool USoulCharacterSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if(GetHealth() <= 0)
	{
		return false;
	}
	return Super::PreGameplayEffectExecute(Data);
}

void USoulCharacterSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Causer = EffectContext.GetEffectCauser();

	if(GetHealth() <= 0)
	{
		OnCharacterDeath.Broadcast(Causer);
	}
	else
	{
		FGameplayEventData Payload;
		Payload.EventTag = SoulGameplayTags::GameplayEvent_Hit;
		GetOwningAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
	
}

void USoulCharacterSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void USoulCharacterSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void USoulCharacterSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	OnSoulAttributeChanged.Broadcast(GetHealthAttribute(), GetHealth(), OldValue.GetCurrentValue());
}

void USoulCharacterSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	OnSoulAttributeChanged.Broadcast(GetMaxHealthAttribute(), GetMaxHealth(), OldValue.GetCurrentValue());
}

void USoulCharacterSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	OnSoulAttributeChanged.Broadcast(GetStaminaAttribute(), GetStamina(), OldValue.GetCurrentValue());
}

void USoulCharacterSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	OnSoulAttributeChanged.Broadcast(GetMaxStaminaAttribute(), GetMaxStamina(), OldValue.GetCurrentValue());
}

void USoulCharacterSet::ClampAttribute(const FGameplayAttribute& Attribute, float NewValue)
{
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if(Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1);
	}
	else if(Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}else if(Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1);
	}
	
}
