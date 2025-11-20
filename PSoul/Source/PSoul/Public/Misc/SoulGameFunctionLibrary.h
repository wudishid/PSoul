// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulGameFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API USoulGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "SoulGameFunctionLibrary")
	static bool IsSameTeam(AActor* InActor1, AActor* InActor2);
	
};
