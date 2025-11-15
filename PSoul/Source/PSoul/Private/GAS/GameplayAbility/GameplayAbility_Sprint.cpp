// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/GameplayAbility_Sprint.h"

#include "GAS/Attribute/SoulCharacterSet.h"

UGameplayAbility_Sprint::UGameplayAbility_Sprint()
{
	ActivationPolicy = ESoulAbilityActivationPolicy::WhileInputActive;
}

void UGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const USoulCharacterSet* Set = GetAbilitySystemComponentFromActorInfo()->GetSet<USoulCharacterSet>())
	{
		if (!Set->OnStaminaEmpty.IsBoundToObject(this))
		{
			Set->OnStaminaEmpty.AddUObject(this, &ThisClass::HandleStaminaEmpty);
		}
	}
}

void UGameplayAbility_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	StopSprint();
}

void UGameplayAbility_Sprint::HandleStaminaEmpty()
{
	StopSprint();
}
