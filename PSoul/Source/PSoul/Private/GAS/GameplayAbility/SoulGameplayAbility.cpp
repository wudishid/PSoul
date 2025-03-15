// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/SoulGameplayAbility.h"

USoulGameplayAbility::USoulGameplayAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
