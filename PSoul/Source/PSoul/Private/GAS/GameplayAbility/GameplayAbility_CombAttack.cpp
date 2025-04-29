// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/GameplayAbility/GameplayAbility_CombAttack.h"
#include "AbilitySystemComponent.h"
#include "../../../../../../../UE5.4.4/UnrealEngine-release/Engine/Plugins/Animation/MotionWarping/Source/MotionWarping/Public/MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PSoul/SoulGameplayTags.h"

void UGameplayAbility_CombAttack::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	
	if (!TriggerEventData) return;

	if (const FGameplayAbilityTargetData_LocationInfo* TargetData_LocationInfo = reinterpret_cast<const FGameplayAbilityTargetData_LocationInfo*>(TriggerEventData->TargetData.Get(0)))
	{
		FVector TempIndexVector = TargetData_LocationInfo->TargetLocation.LiteralTransform.GetLocation();
		//用向量的X表示蒙太奇的索引
		if (!TempIndexVector.IsZero())
		{
			CurrentCombIndex = TempIndexVector.X;
		}
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
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActorFromActorInfo()))
		{
			if (UMotionWarpingComponent* MotionWrapComp = OwnerCharacter->FindComponentByClass<
				UMotionWarpingComponent>())
			{
				FRotator TargetRotation = FRotator(OwnerCharacter->GetActorRotation().Pitch, OwnerCharacter->GetControlRotation().Yaw, OwnerCharacter->GetActorRotation().Roll);
				
				MotionWrapComp->AddOrUpdateWarpTargetFromLocationAndRotation(
					TEXT("AttackRotate"), FVector::Zero(),  TargetRotation);
			}
		}
		
		PlayMontageAndWaitForEvent();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
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
