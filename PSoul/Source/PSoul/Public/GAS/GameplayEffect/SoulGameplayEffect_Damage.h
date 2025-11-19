// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayEffect.h"
#include "SoulGameplayEffect_Damage.generated.h"


class USoulGameplayEffect_Damage;

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "DamageInfo")
	TSubclassOf<USoulGameplayEffect_Damage> DamageEffect;

	UPROPERTY(EditAnywhere, Category = "DamageInfo")
	float DamageImpulse;
};

UCLASS()
class PSOUL_API USoulGameplayEffect_Damage : public USoulGameplayEffect
{
	GENERATED_BODY()
public:
};
