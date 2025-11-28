// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/GameplayAbility/GameplayAbility_Attack.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/SoulCharacterBase.h"

void UGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetOwningActorFromActorInfo());
		if (!SoulCharacter) return;
		FRotator TargetRotation = SoulCharacter->GetDesiredRotation();

		if (UMotionWarpingComponent* MotionWrapComp = GetOwningActorFromActorInfo()->FindComponentByClass<
			UMotionWarpingComponent>())
		{
			MotionWrapComp->AddOrUpdateWarpTargetFromLocationAndRotation(
				TEXT("AttackRotate"), FVector::Zero(), TargetRotation);
		}

		PlayMontageAndWaitForEvent();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	
}

void UGameplayAbility_Attack::PlayMontageAndWaitForEvent()
{
	if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AttackAnimMontage))
	{
		Task->OnCompleted.AddDynamic(this, &ThisClass::UGameplayAbility_Attack::HandleMontageEnded);
		Task->OnBlendOut.AddDynamic(this, &ThisClass::UGameplayAbility_Attack::HandleMontageEnded);

		Task->OnInterrupted.AddDynamic(this, &ThisClass::UGameplayAbility_Attack::HandleAttackCancelled);
		Task->OnCancelled.AddDynamic(this, &ThisClass::UGameplayAbility_Attack::HandleAttackCancelled);

		Task->ReadyForActivation();
	}
}

void UGameplayAbility_Attack::HandleMontageEnded()
{
	K2_EndAbility();
}

void UGameplayAbility_Attack::HandleAttackCancelled()
{
	K2_EndAbility();
}
