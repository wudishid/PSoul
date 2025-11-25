// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "SoulSaveGame.h"
#include "SoulSaveGame_PlayerData.generated.h"


USTRUCT(BlueprintType)
struct FSavedCharacterData
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayAttributeData Health;
	
	UPROPERTY()
	FGameplayAttributeData Stamina;
};

USTRUCT(BlueprintType)
struct FSavedPlayerData
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayAttributeData Level;
	UPROPERTY()
	FGameplayAttributeData NextLevelNeedSoul;
	UPROPERTY()
	FGameplayAttributeData Life;
	UPROPERTY()
	FGameplayAttributeData Strength;
	UPROPERTY()
	FGameplayAttributeData Stamina;
	UPROPERTY()
	FGameplayAttributeData Soul;
};


USTRUCT(BlueprintType)
struct FSavedSingleItemData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName ItemName;

	UPROPERTY()
	int32 ItemAmount = 1;
};



USTRUCT(BlueprintType)
struct FSoulSavedData
{
	GENERATED_BODY()

	UPROPERTY()
	FSavedCharacterData CharacterData;

	UPROPERTY()
	FSavedPlayerData PlayerData;

	UPROPERTY()
	TArray<FSavedSingleItemData> InventoryItemsData;

	UPROPERTY()
	TArray<FName> WornItemData;

	UPROPERTY()
	TArray<FName> UnlockedSkills;

	UPROPERTY()
	TArray<FName> LearnedSkills;

	UPROPERTY()
	TMap<FGameplayTag, FName> QuickSkills;
};


UCLASS()
class PSOUL_API USoulSaveGame_PlayerData : public USoulSaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FSoulSavedData SavedData;
};
