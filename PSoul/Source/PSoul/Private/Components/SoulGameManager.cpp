// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/SoulGameManager.h"
#include "AI/AICharacterBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
USoulGameManager::USoulGameManager()
{
}

void USoulGameManager::StartSpawnAI()
{
	ResetSpawnAIData();
	if (AISpawnDataTable)
	{
		AISpawnDataTable->GetAllRows<FAIRoundSpawnData>(TEXT(""), AIRoundSpawnData);
	}
	NextRound();
}

// Called when the game starts
void USoulGameManager::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void USoulGameManager::SpawnWaveAI()
{
	if (IsCurrentRoundOver())
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_SpawnAI);
		
		if (IsRoundOver())
		{
			GameOver();
		}
		return;
	}
	
	if (OnGetQuerier.IsBound())
	{
		CurrentQueryActor = OnGetQuerier.Execute();
		if (CurrentQueryActor.IsValid())
		{
			if (UEnvQueryInstanceBlueprintWrapper* Wrapper = UEnvQueryManager::RunEQSQuery(
					GetWorld(), AISpawnEQ, CurrentQueryActor.Get(), EEnvQueryRunMode::RandomBest25Pct,
					UEnvQueryInstanceBlueprintWrapper::StaticClass()))
			{
				Wrapper->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnQueryFinished);
			}
		}
	}
}

bool USoulGameManager::IsCurrentRoundOver() const
{
	return CurrentWaveIndex > (AIRoundSpawnData[CurrentRoundIndex]->AIWaveSpawnData.Num() - 1);
}

bool USoulGameManager::IsRoundOver() const
{
	return CurrentRoundIndex > (AIRoundSpawnData.Num() - 1);
}

void USoulGameManager::NextRound()
{
	CurrentRoundIndex++;
	if (IsRoundOver())
	{
		GameOver();
		return;
	}
	CurrentWaveIndex = 0;
	OnRoundStart.Broadcast(CurrentRoundIndex + 1);
	SetSpawnAITimer();
}

void USoulGameManager::GameOver()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_SpawnAI);
	OnGameOver.Broadcast();
}

void USoulGameManager::ResetSpawnAIData()
{
	CurrentWaveIndex = -1;
	CurrentRoundIndex = -1;
	CurrentRoundAIs.Empty();
	AIRoundSpawnData.Empty();
}

void USoulGameManager::SetSpawnAITimer()
{
	GetWorld()->GetTimerManager().SetTimer(TH_SpawnAI, this, &USoulGameManager::SpawnWaveAI, WaveInterval, true, WaveInterval);
}

void USoulGameManager::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                       EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus == EEnvQueryStatus::Success)
	{
		if (QueryInstance)
		{
			if (!AIRoundSpawnData.IsValidIndex(CurrentRoundIndex) || !AIRoundSpawnData[CurrentRoundIndex]->AIWaveSpawnData.IsValidIndex(
				CurrentWaveIndex))
				return;
			
			TArray<FVector> Locations;
			QueryInstance->GetQueryResultsAsLocations(Locations);
			
			if (!Locations.IsEmpty())
			{
				for (auto& AIClass : AIRoundSpawnData[CurrentRoundIndex]->AIWaveSpawnData[CurrentWaveIndex].
				     SpawnedAIClasses)
				{
					if (ensureAlwaysMsgf(AIClass, TEXT("AIClass is not valid!")))
					{
						int32 RandomIndex = FMath::RandRange(0, Locations.Num() - 1);
						FVector SpawnLocation = Locations[RandomIndex];
						FRotator SpawnRotation;
						if (CurrentQueryActor.IsValid())
						{
							SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, CurrentQueryActor.Get()->GetActorLocation());
						}
						FActorSpawnParameters Sp;
						Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
						if (AAICharacterBase* AI = GetWorld()->SpawnActor<AAICharacterBase>(AIClass, SpawnLocation, SpawnRotation, Sp))
						{
							CurrentRoundAIs.Add(AI);
							AI->OnCharacterDied.AddUObject(this, &ThisClass::OnSpawnedAIDied);
						}
					}
				}
				CurrentWaveIndex++;
			}
		}
	}
}

void USoulGameManager::OnSpawnedAIDied(ASoulCharacterBase* InAICharacter)
{
	CurrentRoundAIs.Remove(Cast<AAICharacterBase>(InAICharacter));
	if (CurrentRoundAIs.IsEmpty())
	{
		if (IsCurrentRoundOver())
		{
			NextRound();
		}
	}
}



