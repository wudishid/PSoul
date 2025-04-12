// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/Attribute/SoulPlayerSet.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "Net/UnrealNetwork.h"

USoulPlayerSet::USoulPlayerSet()
{
	InitLevel(1);
	InitNextLevelNeedSoul(0);
	InitLife(1);
	InitStrength(1);
	InitStamina(1);
	InitSoul(0);
}

bool USoulPlayerSet::AddAttributePoint(const FGameplayAttribute& InAttribute)
{
#if WITH_SERVER_CODE

	if (GetSoul() >= GetNextLevelNeedSoul())
	{
		int32 RemainSoul = GetSoul() - GetNextLevelNeedSoul();
		SetSoul(RemainSoul);
		SetLevel(GetLevel() + 1);
		SetNextLevelNeedSoul(GetNextLevelNeedSoul() * 1.2f);
		if (InAttribute == GetLifeAttribute())
		{
			SetLife(GetLife() + 1);
			GetAbilitySystemComponent()->ApplyModToAttribute(USoulCharacterSet::GetMaxHealthAttribute(), EGameplayModOp::Additive, 50);
		}
		else if (InAttribute == GetStaminaAttribute())
		{
			SetStamina(GetStamina() + 1);
			GetAbilitySystemComponent()->ApplyModToAttribute(USoulCharacterSet::GetMaxStaminaAttribute(), EGameplayModOp::Additive, 5);
			GetAbilitySystemComponent()->ApplyModToAttribute(USoulCharacterSet::GetPhysicalDefenceAttribute(), EGameplayModOp::Additive, 3);
		}
		else if (InAttribute == GetStrengthAttribute())
		{
			SetStrength(GetStrength() + 1);
			GetAbilitySystemComponent()->ApplyModToAttribute(USoulCharacterSet::GetPhysicalAttackAttribute(), EGameplayModOp::Additive, 5);
		}
		return true;
	}
	return false;

#endif
}

void USoulPlayerSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USoulPlayerSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulPlayerSet, NextLevelNeedSoul, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulPlayerSet, Life, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulPlayerSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulPlayerSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulPlayerSet, Soul, COND_None, REPNOTIFY_Always);
}

bool USoulPlayerSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void USoulPlayerSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void USoulPlayerSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulPlayerSet, Level, OldValue);
	OnSoulAttributeChanged.Broadcast(GetLevelAttribute(), GetLevel(), OldValue.GetCurrentValue());
}

void USoulPlayerSet::OnRep_NextLevelNeedSoul(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulPlayerSet, NextLevelNeedSoul, OldValue);
	OnSoulAttributeChanged.Broadcast(GetNextLevelNeedSoulAttribute(), GetNextLevelNeedSoul(), OldValue.GetCurrentValue());
}

void USoulPlayerSet::OnRep_Life(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulPlayerSet, Life, OldValue);
	OnSoulAttributeChanged.Broadcast(GetLifeAttribute(), GetLife(), OldValue.GetCurrentValue());
}

void USoulPlayerSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulPlayerSet, Strength, OldValue);
	OnSoulAttributeChanged.Broadcast(GetStrengthAttribute(), GetStrength(), OldValue.GetCurrentValue());
}

void USoulPlayerSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulPlayerSet, Stamina, OldValue);
	OnSoulAttributeChanged.Broadcast(GetStaminaAttribute(), GetStamina(), OldValue.GetCurrentValue());
}

void USoulPlayerSet::OnRep_Soul(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulPlayerSet, Soul, OldValue);
	OnSoulAttributeChanged.Broadcast(GetSoulAttribute(), GetSoul(), OldValue.GetCurrentValue());
}

