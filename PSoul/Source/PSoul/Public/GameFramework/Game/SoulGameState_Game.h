// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulGameState.h"
#include "SoulGameState_Game.generated.h"

UCLASS()
class PSOUL_API ASoulGameState_Game : public ASoulGameState
{
	GENERATED_BODY()
public:
	ASoulGameState_Game();
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
