// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/GameplayAbility/GameplayAbility_Roll.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/SoulCharacterBase.h"


void UGameplayAbility_Roll::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	if (!TriggerEventData) return;

	if (const FGameplayAbilityTargetData_LocationInfo* TargetData_LocationInfo = reinterpret_cast<const FGameplayAbilityTargetData_LocationInfo*>(TriggerEventData->TargetData.Get(0)))
	{
		FRotator RollRotation = TargetData_LocationInfo->TargetLocation.LiteralTransform.Rotator();
		
		//更新motionWarping组件
		if (UMotionWarpingComponent* MontionWarpingComponent = GetOwningActorFromActorInfo()->FindComponentByClass<UMotionWarpingComponent>())
		{
			MontionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("RollRotate", FVector::Zero(), RollRotation);
		}
	}
}

void UGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
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



