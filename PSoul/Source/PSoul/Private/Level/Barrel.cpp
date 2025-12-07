// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Barrel.h"

#include "Util/Util_Common.h"


// Sets default values

ABarrel::ABarrel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}


void ABarrel::SpawnItems_Implementation()
{
	if (!DropedItems.IsEmpty())
	{
		for (auto& DropedItem : DropedItems)
		{
			Util_Common::SpawnInventoryItemInstance(this, DropedItem);
		}
	}
}

// Called when the game starts or when spawned
void ABarrel::BeginPlay()
{
	Super::BeginPlay();
}


