// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulGameState.h"
#include "SoulGameState_Game.generated.h"

class USoulGameManager;



UCLASS()
class PSOUL_API ASoulGameState_Game : public ASoulGameState
{
	GENERATED_BODY()
public:
	ASoulGameState_Game();
	void StartGame();
	bool IsGameStarted() const { return bGameStarted; };
protected:
	AActor* OnGetQuerier();
	void OnRoundStart(int32 RoundNum);
	void OnGameOver();
	
	UPROPERTY(EditDefaultsOnly, Category = "SoulGameManager")
	TObjectPtr<USoulGameManager> GameManagerComp;
protected:
	bool bGameStarted = false;
};
