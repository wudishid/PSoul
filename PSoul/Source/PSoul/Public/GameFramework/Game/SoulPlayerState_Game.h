// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerState.h"
#include "SoulPlayerState_Game.generated.h"

UENUM()
enum class ESoulCharacterTeam : uint8
{
	None = 1,
	RedPlayer = 2,
	BluePlayer = 3,
	AI
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnKillNumberChanged, int32/*击杀次数*/, ESoulCharacterTeam/*阵营*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDeathNumberChanged, int32/*死亡次数*/, ESoulCharacterTeam/*阵营*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamChanged, ESoulCharacterTeam);

UCLASS()
class PSOUL_API ASoulPlayerState_Game : public ASoulPlayerState
{
	GENERATED_BODY()

public:
	void InitPlayerState(int32 InKillNumber, int32 InDeathNumber, ESoulCharacterTeam InTeam);

	void AddKillNumber();
	void AddDeathNumber();

	int32 GetKillNumber() const { return KillNumber; };
	int32 GetDeathNumber() const { return DeathNumber; };
	ESoulCharacterTeam GetTeam() const { return Team; };
	
	FOnKillNumberChanged OnKillNumberChanged;
	FOnDeathNumberChanged OnDeathNumberChanged;
	FOnTeamChanged OnTeamChanged;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_KillNumber();

	UFUNCTION()
	void OnRep_DeathNumber();

	UFUNCTION()
	void OnRep_Team();
	
	UPROPERTY(ReplicatedUsing = OnRep_KillNumber)
	int32 KillNumber;

	UPROPERTY(ReplicatedUsing = OnRep_DeathNumber)
	int32 DeathNumber;

	UPROPERTY(ReplicatedUsing = OnRep_Team)
	ESoulCharacterTeam Team;
	
};
