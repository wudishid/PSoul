// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayEffect/DamageGameplayEffectComponent.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/SoulGameplayAbilityTargetTypes.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/GameplayEffect/SoulGameplayEffect_Damage.h"
#include "PSoul/SoulGameplayTags.h"

void UDamageGameplayEffectComponent::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer,
                                                              FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectExecuted(ActiveGEContainer, GESpec, PredictionKey);
	
	if (UAbilitySystemComponent* ASC = ActiveGEContainer.Owner)
	{
		//未死亡，发送受击事件
		if (ASC->GetSet<USoulCharacterSet>()->GetHealth() > 0)
		{
			EDamageEffect DamageEffectType;
			if (const USoulGameplayEffect_Damage* GE_Damage = Cast<USoulGameplayEffect_Damage>(GESpec.Def))
			{
				DamageEffectType = GE_Damage->DamageEffect;
			}
			
			//根据伤害效果来判断
			if (DamageEffectType == EDamageEffect::Normal)
			{
				FGameplayEventData Payload;
				FGameplayAbilityTargetData_DamageInfo* DamageInfo = new FGameplayAbilityTargetData_DamageInfo();
				DamageInfo->Impulse = Impulse;
				Payload.TargetData.Add(DamageInfo);
				Payload.Instigator = GESpec.GetEffectContext().GetEffectCauser();
				Payload.EventTag = SoulGameplayTags::GameplayEvent_Hit;
				ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
			}
			else if (DamageEffectType == EDamageEffect::Nudge)
			{
				FGameplayEventData Payload;
				FGameplayAbilityTargetData_DamageInfo* DamageInfo = new FGameplayAbilityTargetData_DamageInfo();
				DamageInfo->Impulse = Impulse;
				Payload.TargetData.Add(DamageInfo);
				Payload.Instigator = GESpec.GetEffectContext().GetEffectCauser();
				Payload.EventTag = SoulGameplayTags::GameplayEvent_Nudge;
				ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
			}
			else if (DamageEffectType == EDamageEffect::KnockUp)
			{
				FGameplayEventData Payload;
				Payload.Instigator = GESpec.GetEffectContext().GetEffectCauser();
				Payload.EventTag = SoulGameplayTags::GameplayEvent_KnockUp;
				ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
			}
		}
	}
}
