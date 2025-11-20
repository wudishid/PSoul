// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SoulAbilitySystemBlueprintLibrary.h"

bool USoulAbilitySystemBlueprintLibrary::GetTargetData_DamageInfoFromTargetData(
	const FGameplayAbilityTargetDataHandle& TargetData, FGameplayAbilityTargetData_DamageInfo& OutTargetData_DamageInfo)
{
	if (!TargetData.Data.IsEmpty())
	{
		for (int32 i = 0; i < TargetData.Data.Num(); i++)
		{
			if (FGameplayAbilityTargetData_DamageInfo* TargetData_DamageInfo = reinterpret_cast<FGameplayAbilityTargetData_DamageInfo*>(TargetData.Data[i].Get()))
			{
				OutTargetData_DamageInfo = *TargetData_DamageInfo;
				return true;
			}
		}
	}
	
	return false;
}
