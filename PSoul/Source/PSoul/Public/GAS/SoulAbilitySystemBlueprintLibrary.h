// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbilityTargetTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulAbilitySystemBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API USoulAbilitySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Ability|TargetData")
	static bool GetTargetData_DamageInfoFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayAbilityTargetData_DamageInfo& OutTargetData_DamageInfo);
};
