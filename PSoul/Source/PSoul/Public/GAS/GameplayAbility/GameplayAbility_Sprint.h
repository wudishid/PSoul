// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "GameplayAbility_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API UGameplayAbility_Sprint : public USoulGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	void HandleStaminaEmpty();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StopSprint();
};
