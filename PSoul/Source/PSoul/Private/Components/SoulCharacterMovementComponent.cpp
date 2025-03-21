// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SoulCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "PSoul/SoulGameplayTags.h"

USoulCharacterMovementComponent::USoulCharacterMovementComponent()
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	MaxWalkSpeed = 300.f;
}

float USoulCharacterMovementComponent::GetMaxSpeed() const
{
	if(UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if(ASC->HasMatchingGameplayTag(SoulGameplayTags::Status_Run))
		{
			return 600.f;
		}
	}
	
	return Super::GetMaxSpeed();
}

void USoulCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


