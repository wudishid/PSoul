// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SoulGameplayAbility.generated.h"


UENUM(BlueprintType)
enum class ESoulAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UCLASS()
class PSOUL_API USoulGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	ESoulAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; };

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ActivationPolicy")
	ESoulAbilityActivationPolicy ActivationPolicy;
	
};
