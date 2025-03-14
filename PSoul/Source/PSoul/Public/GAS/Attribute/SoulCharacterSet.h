// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SoulAttributeSet.h"
#include "SoulCharacterSet.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API USoulCharacterSet : public USoulAttributeSet
{
	GENERATED_BODY()

	ATTRIBUTE_ACCESSORS(USoulCharacterSet, Health);
	
public:
	USoulCharacterSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_MaxHealth();

	UFUNCTION()
	void OnRep_Stamina();

	UFUNCTION()
	void OnRep_MaxStamina();
	
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	
};
