// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API UGameplayAbility_Attack : public USoulGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	void PlayMontageAndWaitForEvent();

	UFUNCTION()
	void HandleMontageEnded();

	UFUNCTION()
	void HandleAttackCancelled();
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AttackAnimMontage;
};
