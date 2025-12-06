// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Execution/SoulProjectileDamageEffectExecution.h"

#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/GameplayEffect/SoulGameplayEffect_Damage_Projectile.h"

void USoulProjectileDamageEffectExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	if (const USoulGameplayEffect_Damage_Projectile* Damage_Projectile = Cast<USoulGameplayEffect_Damage_Projectile>(ExecutionParams.GetOwningSpec().Def))
	{
		float CauserPhysicalAttack = Damage_Projectile->PhysicalDamage;
		float CauserMagicAttack = Damage_Projectile->MagicDamage;
		float TargetPhysicalDefence = ExecutionParams.GetTargetAbilitySystemComponent()->GetSet<USoulCharacterSet>()->GetPhysicalDefence();
		float TargetMagicDefence = ExecutionParams.GetTargetAbilitySystemComponent()->GetSet<USoulCharacterSet>()->GetMagicDefence();
		float RealDamage = FMath::Max(CauserPhysicalAttack - TargetPhysicalDefence, 0) + FMath::Max(CauserMagicAttack - TargetMagicDefence, 0);
		
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USoulCharacterSet::GetDamageAttribute(), EGameplayModOp::Additive, RealDamage));
	}
#endif
	
}
