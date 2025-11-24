// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

class AInventoryItemInstance;

namespace Util_Common
{
	bool SpawnInventroyItemInstance(AActor* OwnerActor, TSubclassOf<AInventoryItemInstance> ItemClass);
	void PlayOpenPanelSound(UWorld* InWorld);
	void PlayClosePanelSound(UWorld* InWorld);
}
