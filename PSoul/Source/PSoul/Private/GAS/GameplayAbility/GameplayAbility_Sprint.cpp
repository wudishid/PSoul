// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/GameplayAbility_Sprint.h"

void UGameplayAbility_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	StopSprint();
}
