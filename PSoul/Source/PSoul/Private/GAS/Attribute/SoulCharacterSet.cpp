// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/SoulCharacterSet.h"

#include "Net/UnrealNetwork.h"

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
	return Super::PreGameplayEffectExecute(Data);
}

void USoulCharacterSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void USoulCharacterSet::OnRep_Health()
{
	
}

void USoulCharacterSet::OnRep_MaxHealth()
{
	
}

void USoulCharacterSet::OnRep_Stamina()
{
	
}

void USoulCharacterSet::OnRep_MaxStamina()
{
	
}
