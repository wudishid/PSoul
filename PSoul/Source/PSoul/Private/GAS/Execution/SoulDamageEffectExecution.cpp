// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Execution/SoulDamageEffectExecution.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "PSoul/SoulGameplayTags.h"

void USoulDamageEffectExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	float CauserPhysicalAttack = ExecutionParams.GetSourceAbilitySystemComponent()->GetSet<USoulCharacterSet>()->GetPhysicalAttack();
	float TargetPhysicalDefence = ExecutionParams.GetTargetAbilitySystemComponent()->GetSet<USoulCharacterSet>()->GetPhysicalDefence();
	float RealDamage = FMath::Max(CauserPhysicalAttack - TargetPhysicalDefence, 0.f);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USoulCharacterSet::GetDamageAttribute(), EGameplayModOp::Additive, RealDamage));
	
#endif
}
