// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AISpawnPoint.generated.h"

UCLASS()
class PSOUL_API AAISpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAISpawnPoint();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

protected:
	UPROPERTY(EditAnywhere, Category= "")
};
