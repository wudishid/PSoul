// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "UObject/Object.h"
#include "SoulGameplayAbilityTargetTypes.generated.h"

USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_DamageInfo : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = TargetData_DamageInfo)
	float Impulse;
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_DamageInfo::StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FGameplayAbilityTargetData_DamageInfo");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_DamageInfo> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_DamageInfo>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};


USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_AttackInfo : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	int32 CombAttackIndex = 0;

	UPROPERTY()
	FVector InputVector;
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_AttackInfo::StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FGameplayAbilityTargetData_AttackInfo");
	}
	
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_AttackInfo> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_AttackInfo>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};

