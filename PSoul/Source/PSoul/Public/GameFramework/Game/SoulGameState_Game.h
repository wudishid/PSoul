// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulGameState.h"
#include "SoulGameState_Game.generated.h"


enum class ESoulCharacterTeam : uint8;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameTimeLengthChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRedPlayersScoreChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBluePlayersScoreChanged, int32);

UCLASS()
class PSOUL_API ASoulGameState_Game : public ASoulGameState
{
	GENERATED_BODY()

public:
	ASoulGameState_Game();
	
	FOnGameTimeLengthChanged OnGameTimeLengthChanged;
	FOnRedPlayersScoreChanged OnRedPlayersScoreChanged;
	FOnBluePlayersScoreChanged OnBluePlayersScoreChanged;

	int32 GetRedPlayersScore() const { return RedPlayersScore; }
	int32 GetBluePlayersScore() const { return BluePlayersScore; }
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_GameTimeLength();

	UFUNCTION()
	void OnRep_RedPlayersScore();

	UFUNCTION()
	void OnRep_BluePlayersScore();
	
	void UpdatePlayerKillDeathNumber();
	
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_GameTimeLength, Category = "SoulGameState")
	float GameTimeLength;

	UPROPERTY(ReplicatedUsing = OnRep_RedPlayersScore)
	int32 RedPlayersScore;

	UPROPERTY(ReplicatedUsing = OnRep_BluePlayersScore)
	int32 BluePlayersScore;
	
	FTimerHandle Timer_GameTimeLength;
	
};
