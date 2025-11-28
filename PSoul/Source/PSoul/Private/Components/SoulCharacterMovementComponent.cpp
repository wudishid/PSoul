// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/SoulCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "PSoul/SoulGameplayTags.h"

USoulCharacterMovementComponent::USoulCharacterMovementComponent()
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	MaxWalkSpeed = 300.f;
}

float USoulCharacterMovementComponent::GetMaxSpeed() const
{
	if (ASC->HasMatchingGameplayTag(SoulGameplayTags::Status_Run))
	{
		return DefaultMaxSpeed;
	}
	
	return Super::GetMaxSpeed();
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



