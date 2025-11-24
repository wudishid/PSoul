// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulGameFunctionLibrary.generated.h"

struct FGameplayEventData;
class USoulGameplayAbility;
class AInventoryItemInstance;

UENUM(BlueprintType)
enum class EEventDirection : uint8
{
	Forward = 0			UMETA(DisplayName = "前"),
	Back  				UMETA(DisplayName = "后")
};

UCLASS()
class PSOUL_API USoulGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "SoulGameFunctionLibrary")
	static bool IsSameTeam(AActor* InActor1, AActor* InActor2);

	UFUNCTION(BlueprintCallable, Category = "SoulGameFunctionLibrary")
	static bool SpawnInventroyItemInstance(AActor* OwnerActor, TSubclassOf<AInventoryItemInstance> ItemClass);

	UFUNCTION(BlueprintPure, Category = "SoulGameFunctionLibrary", meta = (DefaultToSelf = "InAbility") )
	static EEventDirection GetEventDataDirection(const FGameplayEventData& InEventData, USoulGameplayAbility* InAbility);
	
};
