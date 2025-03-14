// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SoulAbilitySet.h"

#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/GameplayAbility/SoulGameplayAbility.h"
#include "PSoul/SoulLog.h"

USoulAbilitySet::USoulAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void USoulAbilitySet::GiveToAbilitySystem(USoulAbilitySystemComponent* ASC) const
{
	check(ASC);

	for (const FSoulAbilitySet_GameplayAbility& AbilityToGranted : GrantedGameplayAbilities)
	{
		if (!IsValid(AbilityToGranted.Ability))
		{
			UE_LOG(LogSoulAbilitySystem, Warning, TEXT("%s ability is null"), *GetNameSafe(this));
		}

		USoulGameplayAbility* Ability = AbilityToGranted.Ability.GetDefaultObject();
		FGameplayAbilitySpec AbilitySpec(Ability, AbilityToGranted.AbilityLevel);
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGranted.InputTag);
		ASC->GiveAbility(AbilitySpec);
	}

	for(const FSoulAbilitySet_GameplayEffect& EffectToGranted : GrantedGameplayEffects)
	{
		if (!IsValid(EffectToGranted.GameplayEffect))
		{
			UE_LOG(LogSoulAbilitySystem, Warning, TEXT("%s effect is null"), *GetNameSafe(this));
		}

		UGameplayEffect* Effect = EffectToGranted.GameplayEffect.GetDefaultObject();
		ASC->ApplyGameplayEffectToSelf(Effect, EffectToGranted.EffectLevel, ASC->MakeEffectContext());
	}

	for(const FSoulAbilitySet_AttributeSet& AttributeToGranted : GrantedAttributes)
	{
		if (!IsValid(AttributeToGranted.AttributeSet))
		{
			UE_LOG(LogSoulAbilitySystem, Warning, TEXT("%s attribute is null"), *GetNameSafe(this));
		}

		UAttributeSet* AttributeSet =NewObject<UAttributeSet>(ASC->GetOwner(), AttributeToGranted.AttributeSet);
		ASC->AddAttributeSetSubobject(AttributeSet);
	}
	
}
