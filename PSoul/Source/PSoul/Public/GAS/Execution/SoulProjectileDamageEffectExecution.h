// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SoulProjectileDamageEffectExecution.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API USoulProjectileDamageEffectExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
