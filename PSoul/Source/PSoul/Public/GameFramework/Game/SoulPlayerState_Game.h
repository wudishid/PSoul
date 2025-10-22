// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerState.h"
#include "SoulPlayerState_Game.generated.h"


UCLASS()
class PSOUL_API ASoulPlayerState_Game : public ASoulPlayerState
{
	GENERATED_BODY()
public:
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
