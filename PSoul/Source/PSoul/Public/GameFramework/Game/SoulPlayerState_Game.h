// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerState.h"
#include "SoulPlayerState_Game.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChanged, FString);

UCLASS()
class PSOUL_API ASoulPlayerState_Game : public ASoulPlayerState
{
	GENERATED_BODY()

public:
	FOnPlayerNameChanged OnPlayerNameChanged;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerName() override;
};
