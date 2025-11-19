// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SoulGameplayAbilityTargetTypes.h"

bool FGameplayAbilityTargetData_DamageInfo::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	if (Ar.IsSaving())
	{
		bool success = true;
		success &= WriteFixedCompressedFloat<1, 16>(Impulse, Ar);
		return success;
	}
	else
	{
		ReadFixedCompressedFloat<1, 16>(Impulse, Ar);
		return true;
	}
}
