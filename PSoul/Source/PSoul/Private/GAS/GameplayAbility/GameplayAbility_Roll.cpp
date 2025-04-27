// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/GameplayAbility/GameplayAbility_Roll.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/SoulCharacterBase.h"


void UGameplayAbility_Roll::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	if (!TriggerEventData) return;

	if (const FGameplayAbilityTargetData_LocationInfo* TargetData_LocationInfo = reinterpret_cast<const FGameplayAbilityTargetData_LocationInfo*>(TriggerEventData->TargetData.Get(0)))
	{
		FRotator RollRotation = TargetData_LocationInfo->TargetLocation.LiteralTransform.Rotator();
		HasRollRotation = !RollRotation.IsZero();
		if (HasRollRotation)
		{
			//更新motionWarping组件
			if (UMotionWarpingComponent* MontionWarpingComponent = GetOwningActorFromActorInfo()->FindComponentByClass<UMotionWarpingComponent>())
			{
				MontionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("RollRotate", FVector::Zero(), RollRotation);
			}
		}
	}
}

UAnimMontage* UGameplayAbility_Roll::GetRollMontage() const
{
	return HasRollRotation ? RollMontage : SkipMontage;
}


