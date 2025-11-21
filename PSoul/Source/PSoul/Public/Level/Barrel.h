// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SoulDamageInterface.h"
#include "Barrel.generated.h"

class AInventoryItemInstance;

UCLASS()
class PSOUL_API ABarrel : public AActor, public ISoulDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABarrel(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Barrel")
	void SpawnItems();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Barrel")
	TArray<TSubclassOf<AInventoryItemInstance>> DropedItems;
};
