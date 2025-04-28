// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulAttributeSet.h"
#include "SoulCharacterSet.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSoulCharacterDeath, AActor*/*Causer*/);
DECLARE_MULTICAST_DELEGATE(FOnStaminaEmpty);

UCLASS(BlueprintType)
class PSOUL_API USoulCharacterSet : public USoulAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, Health);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, Stamina);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, MaxStamina);

	ATTRIBUTE_ACCESSORS(USoulCharacterSet, PhysicalAttack);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, PhysicalDefence);
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, Toughness);
	
	ATTRIBUTE_ACCESSORS(USoulCharacterSet, Damage);
public:
	USoulCharacterSet();

	mutable  FOnSoulCharacterDeath OnCharacterDeath;
	mutable  FOnStaminaEmpty OnStaminaEmpty;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_PhysicalAttack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_PhysicalDefence(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Toughness(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldValue);
	
	void ClampAttribute(const FGameplayAttribute& Attribute, float NewValue);
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalAttack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PhysicalAttack;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalDefence, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PhysicalDefence;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Toughness, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Toughness;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	
};
