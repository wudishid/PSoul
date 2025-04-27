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
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Ability_Roll")
	UAnimMontage* GetRollMontage() const;
protected:
	UPROPERTY(EditAnywhere, Category = "Roll")
	UAnimMontage* RollMontage;

	UPROPERTY(EditAnywhere, Category = "Roll")
	UAnimMontage* SkipMontage;

	bool HasRollRotation;
};
