// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AICharacterDataAsset.generated.h"

class AInventoryItemInstance;

USTRUCT()
struct FDropItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (UIMin = 0, ClampMin = 0, UIMax = 1, ClampMax = 1))
	float DropProbability = 0.5f;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AInventoryItemInstance>> DropedItems;
};


UCLASS()
class PSOUL_API UAICharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "DropItem", meta = (UIMin = 0, ClampMin = 0, UIMax = 10, ClampMax = 10))
	int32 MaxDropItemAmount = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "DropItem")
	TArray<FDropItemInfo> DropedItemsInfo;
};
