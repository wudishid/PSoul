// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulCharacterBase.h"
#include "AICharacterBase.generated.h"

class UAICharacterDataAsset;
class UMotionWarpingComponent;

UCLASS()
class PSOUL_API AAICharacterBase : public ASoulCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacterBase();

protected:
	UPROPERTY(EditAnywhere, Category = "AICharcterData")
	TObjectPtr<UAICharacterDataAsset> AICharacterData;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void FinishDeath() override;
};
