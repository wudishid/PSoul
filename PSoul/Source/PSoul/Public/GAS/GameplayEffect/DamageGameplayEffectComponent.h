// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "DamageGameplayEffectComponent.generated.h"

enum class EDamageEffect : uint8;
/**
 * 
 */
UCLASS()
class PSOUL_API UDamageGameplayEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()
public:
	virtual void OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
	
	//伤害冲量
	float Impulse = 100.f;
};
