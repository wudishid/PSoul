// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SoulAttributeSet.h"
#include "SoulCharacterSet.generated.h"




UCLASS(BlueprintType)
class PSOUL_API USoulCharacterSet : public USoulAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, Health);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, Stamina);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, MaxStamina);
public:
	USoulCharacterSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	
};
