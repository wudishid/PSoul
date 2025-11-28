// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/SoulCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "PSoul/SoulGameplayTags.h"

USoulCharacterMovementComponent::USoulCharacterMovementComponent()
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

float USoulCharacterMovementComponent::GetMaxSpeed() const
{
	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(SoulGameplayTags::Status_Run))
		{
			return ASC->GetSet<USoulCharacterSet>()->GetMaxMoveSpeed();
		}
		else
		{
			return ASC->GetSet<USoulCharacterSet>()->GetMaxMoveSpeed() / 2;
		}
	}
	
	return Super::GetMaxSpeed();
}

float USoulCharacterMovementComponent::GetDefaultMaxSpeed() const
{
	if (ASC)
	{
		return ASC->GetSet<USoulCharacterSet>()->GetMaxMoveSpeed();
	}
	
	return GetMaxSpeed();
}

void USoulCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	ASC = Cast<USoulAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
	check(ASC);
	// if (!ASC->RegisterGenericGameplayTagEvent().IsBoundToObject(this))
	// {
	// 	ASC->RegisterGenericGameplayTagEvent().AddUObject(this, &ThisClass::HandleGameplayTagChanged);
	// }
}



