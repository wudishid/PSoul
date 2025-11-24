// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "SoulSaveGame.h"
#include "SoulSaveGame_PlayerData.generated.h"


USTRUCT(BlueprintType)
struct FSavedGameplayAttributeData
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayAttribute Attribute;
	
	UPROPERTY()
	FGameplayAttributeData AttributeData;
};




UCLASS()
class PSOUL_API USoulSaveGame_PlayerData : public USoulSaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FSavedGameplayAttributeData TestAttributeData;
};
