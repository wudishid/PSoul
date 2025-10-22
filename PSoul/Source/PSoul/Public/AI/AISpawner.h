// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AISpawner.generated.h"

class AAICharacterBase;

UCLASS()
class PSOUL_API AAISpawner : public ATargetPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAISpawner();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
protected:
	UPROPERTY(EditAnywhere, Category= "AISpawner")
	bool bEnabled = true;
	
	UPROPERTY(EditAnywhere, Category= "AISpawner")
	float SpawnDelay = 0.1f;
	
	UPROPERTY(EditAnywhere, Category= "AISpawner")
	TSubclassOf<AAICharacterBase> AIClass;

private:
	FTimerHandle SpawnTimer;
};
