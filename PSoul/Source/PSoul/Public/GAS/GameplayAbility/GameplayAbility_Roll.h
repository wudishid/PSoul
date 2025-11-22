// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "GameplayAbility_Roll.generated.h"

UCLASS()
class PSOUL_API UGameplayAbility_Roll : public USoulGameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbility_Roll();
	
protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void PlayMontageAndWaitForEvent();

	UFUNCTION()
	void HandleMontageEnded();

	UFUNCTION()
	void HandleAttackCancelled();
protected:
	UPROPERTY(EditAnywhere, Category = "Roll")
	TObjectPtr<UAnimMontage> RollMontage;
	
};
