// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayEffect_Damage.h"
#include "SoulGameplayEffect_Damage_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API USoulGameplayEffect_Damage_Projectile : public USoulGameplayEffect_Damage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "DamageInfo", meta = (UIMin = 0))
	float PhysicalDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "DamageInfo", meta = (UIMin = 0))
	float MagicDamage = 0.f;
};
