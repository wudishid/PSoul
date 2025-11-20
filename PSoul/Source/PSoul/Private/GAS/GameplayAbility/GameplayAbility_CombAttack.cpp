// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/GameplayAbility/GameplayAbility_CombAttack.h"
#include "AbilitySystemComponent.h"
#include "../../../../../../../UE5.4.4/UnrealEngine-release/Engine/Plugins/Animation/MotionWarping/Source/MotionWarping/Public/MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulGameplayAbilityTargetTypes.h"
#include "PSoul/SoulGameplayTags.h"

void UGameplayAbility_CombAttack::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	
	if (!TriggerEventData) return;
	if (const FGameplayAbilityTargetData_AttackInfo* AttackInfo = reinterpret_cast<const FGameplayAbilityTargetData_AttackInfo*>(TriggerEventData->TargetData.Get(0)))
	{
		CurrentCombIndex = AttackInfo->CombAttackIndex;
	}
}

void UGameplayAbility_CombAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		//更新角色攻击旋转方向
		if (TriggerEventData)
		{
			if (const FGameplayAbilityTargetData_AttackInfo* AttackInfo = reinterpret_cast<const
				FGameplayAbilityTargetData_AttackInfo*>(TriggerEventData->TargetData.Get(0)))
			{
				ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetOwningActorFromActorInfo());
				if (!SoulCharacter) return;
				FRotator TargetRotation = SoulCharacter->GetDesiredRotation();
				if (!AttackInfo->InputVector.IsZero())
				{
					//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, FString::Printf(TEXT("InputVector: %s"), *AttackInfo->InputVector.ToString()));
					TargetRotation.Yaw = AttackInfo->InputVector.Rotation().Yaw;
				}
				if (UMotionWarpingComponent* MotionWrapComp = GetOwningActorFromActorInfo()->FindComponentByClass<
					UMotionWarpingComponent>())
				{
					MotionWrapComp->AddOrUpdateWarpTargetFromLocationAndRotation(
						TEXT("AttackRotate"), FVector::Zero(), TargetRotation);
				}
			}
		}
		PlayMontageAndWaitForEvent();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

bool UGameplayAbility_CombAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGameplayAbility_CombAttack::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbility(Handle);
}

void UGameplayAbility_CombAttack::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void UGameplayAbility_CombAttack::PlayMontageAndWaitForEvent()
{
	if(UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GetCurrentCombMontage()))
	{
		PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageEnded);
		PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageEnded);

		PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleAttackCancelled);
		PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::HandleAttackCancelled);
		
		PlayMontageAndWaitTask->ReadyForActivation();
	}

	//监听通知窗口事件
	if(UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, SoulGameplayTags::GameplayEvent_Montage_OpenCombWindow))
	{
		WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::HandleOpenCombWindow);
		WaitEventTask->ReadyForActivation();
	}
	if(UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, SoulGameplayTags::GameplayEvent_Montage_CloseCombWindow))
	{
		WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::HandleCloseCombWindow);
		WaitEventTask->ReadyForActivation();
	}
	
}

void UGameplayAbility_CombAttack::HandleOpenCombWindow(FGameplayEventData Payload)
{
	bComb = true;
}

void UGameplayAbility_CombAttack::HandleCloseCombWindow(FGameplayEventData Payload)
{
	bComb = false;
}

void UGameplayAbility_CombAttack::HandleMontageEnded()
{
	bComb =false;
	CurrentCombIndex = 0;
	K2_EndAbility();
}

void UGameplayAbility_CombAttack::HandleAttackCancelled()
{
	bComb =false;
	CurrentCombIndex = 0;
	K2_CancelAbility();
}

UGameplayAbility_CombAttack::UGameplayAbility_CombAttack()
	: CurrentCombIndex(0), bComb(false)
{
	ActivationPolicy = ESoulAbilityActivationPolicy::OnInputTriggered;
	bRetriggerInstancedAbility = true;
}

UAnimMontage* UGameplayAbility_CombAttack::GetCurrentCombMontage()
{
	if (!CombAttackInfos.IsEmpty())
	{
		for (const FCombAttackInfo& info : CombAttackInfos)
		{
			if (info.CombIndex == CurrentCombIndex)
			{
				return info.Montage;
			}
		}
	}
	return nullptr;
}
