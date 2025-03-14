// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SoulAbilitySet.generated.h"


class USoulAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class USoulGameplayAbility;


/**
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FSoulAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USoulGameplayAbility> Ability = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
	
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FSoulAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};


/**
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FSoulAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

	//Attribute Init Data
	UPROPERTY(EditDefaultsOnly)
	UDataTable* AttributeSetDataTable = nullptr;
};


UCLASS()
class PSOUL_API USoulAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USoulAbilitySet(const FObjectInitializer& ObjectInitializer);
	
	void GiveToAbilitySystem(USoulAbilitySystemComponent* ASC) const;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FSoulAbilitySet_GameplayAbility> GrantedGameplayAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FSoulAbilitySet_GameplayEffect> GrantedGameplayEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FSoulAbilitySet_AttributeSet> GrantedAttributes;
};
