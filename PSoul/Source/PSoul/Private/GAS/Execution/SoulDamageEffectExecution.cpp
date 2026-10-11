// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/Execution/SoulDamageEffectExecution.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/GameplayEffect/SoulGameplayEffect_Damage.h"

void USoulDamageEffectExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	if (const USoulGameplayEffect_Damage* DamageEffect = Cast<USoulGameplayEffect_Damage>(
		ExecutionParams.GetOwningSpec().Def))
	{
		float CauserPhysicalAttack = ExecutionParams.GetSourceAbilitySystemComponent()->GetSet<USoulCharacterSet>()->
		                                             GetPhysicalAttack() * DamageEffect->DamageMultiply;
		float TargetPhysicalDefence = ExecutionParams.GetTargetAbilitySystemComponent()->GetSet<USoulCharacterSet>()->
		                                              GetPhysicalDefence();
		float RealDamage = FMath::Max(CauserPhysicalAttack - TargetPhysicalDefence, 0.f);
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(USoulCharacterSet::GetDamageAttribute(), EGameplayModOp::Additive,
			                               RealDamage));
	}

#endif
}
