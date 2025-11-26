// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayEffect.h"
#include "SoulGameplayEffect_Damage.generated.h"


class USoulGameplayEffect_Damage;


UENUM(BlueprintType)
enum class EDamageEffect : uint8
{
	Normal = 0			UMETA(DisplayName = "普通"),
	//轻推，比如翻滚碰到敌人
	Nudge				UMETA(DisplayName = "轻推"),
	KnockUp				UMETA(DisplayName = "击飞")
};


USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "DamageInfo")
	TSubclassOf<USoulGameplayEffect_Damage> DamageEffect;
	
	//击退敌人的冲量
	UPROPERTY(EditAnywhere, Category = "DamageInfo")
	float DamageImpulse = 100.f;
};

UCLASS()
class PSOUL_API USoulGameplayEffect_Damage : public USoulGameplayEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "DamageInfo")
	EDamageEffect DamageEffect;
};
