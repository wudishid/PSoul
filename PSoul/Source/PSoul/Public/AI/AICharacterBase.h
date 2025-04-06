// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulCharacterBase.h"
#include "AICharacterBase.generated.h"

class UMotionWarpingComponent;

UCLASS()
class PSOUL_API AAICharacterBase : public ASoulCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacterBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpComp;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void FinishDeath() override;
};
