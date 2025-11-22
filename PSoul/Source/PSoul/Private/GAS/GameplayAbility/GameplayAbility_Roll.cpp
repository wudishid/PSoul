// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/GameplayAbility/GameplayAbility_Roll.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulGameplayAbilityTargetTypes.h"


UGameplayAbility_Roll::UGameplayAbility_Roll()
{
	ActivationPolicy = ESoulAbilityActivationPolicy::OnInputTriggered;
}

void UGameplayAbility_Roll::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetOwningActorFromActorInfo());
		if (!SoulCharacter) return;
		FRotator TargetRotation = SoulCharacter->GetDesiredRotation();
		
		if (TriggerEventData)
		{
			if (const FGameplayAbilityTargetData_AttackInfo* AttackInfo = reinterpret_cast<const
				FGameplayAbilityTargetData_AttackInfo*>(TriggerEventData->TargetData.Get(0)))
			{
				
				if (!AttackInfo->InputVector.IsZero())
				{
					TargetRotation.Yaw = AttackInfo->InputVector.Rotation().Yaw;
				}
			}
		}

		//更新角色攻击旋转方向
		if (UMotionWarpingComponent* MotionWrapComp = GetOwningActorFromActorInfo()->FindComponentByClass<
					UMotionWarpingComponent>())
		{
			MotionWrapComp->AddOrUpdateWarpTargetFromLocationAndRotation(
				TEXT("RollRotate"), FVector::Zero(), TargetRotation);
		}
		
		PlayMontageAndWaitForEvent();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	
}

void UGameplayAbility_Roll::PlayMontageAndWaitForEvent()
{
	if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, RollMontage))
	{
		Task->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageEnded);
		Task->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageEnded);

		Task->OnInterrupted.AddDynamic(this, &ThisClass::HandleAttackCancelled);
		Task->OnCancelled.AddDynamic(this, &ThisClass::HandleAttackCancelled);

		Task->ReadyForActivation();
	}

	

	
}

void UGameplayAbility_Roll::HandleMontageEnded()
{
	K2_EndAbility();
}

void UGameplayAbility_Roll::HandleAttackCancelled()
{
	K2_EndAbility();
}



