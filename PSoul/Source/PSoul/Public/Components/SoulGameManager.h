// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SoulGameManager.generated.h"


class ASoulCharacterBase;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class AAICharacterBase;

USTRUCT(BlueprintType)
struct FAIWaveSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WaveSpawnData")
	TArray<TSubclassOf<AAICharacterBase>> SpawnedAIClasses;
};

USTRUCT(BlueprintType)
struct FAIRoundSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "RoundSpawnData")
	TArray<FAIWaveSpawnData> AIWaveSpawnData;
};

DECLARE_DELEGATE_RetVal(AActor*,FOnGetQuerier);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundStart, int32);
DECLARE_MULTICAST_DELEGATE(FOnGameOver);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API USoulGameManager : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	USoulGameManager();

	FOnGetQuerier OnGetQuerier;
	FOnRoundStart OnRoundStart;
	FOnGameOver OnGameOver;
	
	void StartSpawnAI();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	void SpawnWaveAI();

	bool IsCurrentRoundOver() const;

	bool IsRoundOver() const;
	
	void NextRound();

	void GameOver();

	void ResetSpawnAIData();

	void SetSpawnAITimer();
	
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnSpawnedAIDied(ASoulCharacterBase* InAICharacter);
protected:
	UPROPERTY(EditAnywhere, Category = "SpawnAIData")
	TObjectPtr<UDataTable> AISpawnDataTable;

	UPROPERTY(EditAnywhere, Category = "SpawnAIData")
	TObjectPtr<UEnvQuery> AISpawnEQ;
	
	UPROPERTY(EditAnywhere, Category = "SpawnAIData")
	float WaveInterval = 7.f;

private:
	int32 CurrentWaveIndex = -1;
	int32 CurrentRoundIndex = -1;
	TWeakObjectPtr<AActor> CurrentQueryActor;
	TArray<TWeakObjectPtr<AAICharacterBase>> CurrentRoundAIs;
	TArray<FAIRoundSpawnData*> AIRoundSpawnData;
	FTimerHandle TH_SpawnAI;
};
