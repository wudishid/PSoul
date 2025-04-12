// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulAttributeSet.h"
#include "SoulPlayerSet.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API USoulPlayerSet : public USoulAttributeSet
{
	GENERATED_BODY()

public:
	USoulPlayerSet();

	ATTRIBUTE_ACCESSORS(USoulPlayerSet, Level);
	ATTRIBUTE_ACCESSORS(USoulPlayerSet, NextLevelNeedSoul);
	ATTRIBUTE_ACCESSORS(USoulPlayerSet, Life);
	ATTRIBUTE_ACCESSORS(USoulPlayerSet, Strength);
	ATTRIBUTE_ACCESSORS(USoulPlayerSet, Stamina);
	ATTRIBUTE_ACCESSORS(USoulPlayerSet, Soul);

	bool AddAttributePoint(const FGameplayAttribute& InAttribute);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

protected:
	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_NextLevelNeedSoul(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Life(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Soul(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Level, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Level;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_NextLevelNeedSoul, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData NextLevelNeedSoul;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Life, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Life;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing =  OnRep_Strength, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Soul, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Soul;
};
