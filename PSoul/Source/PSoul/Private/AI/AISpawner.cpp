// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/AISpawner.h"
#include "AI/AICharacterBase.h"


// Sets default values
AAISpawner::AAISpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bEnabled)
	{
		if (HasAuthority())
		{
			if (ensureAlwaysMsgf(AIClass, TEXT("AIClass is not valid!")))
			{
				GetWorld()->GetTimerManager().SetTimer(SpawnTimer, [this]()
				{
					FActorSpawnParameters sp;
					sp.Owner = GetOwner();
					GetWorld()->SpawnActor(AIClass, &GetActorTransform(), sp);
				}, SpawnDelay, false);
			}
		}
	}
}



