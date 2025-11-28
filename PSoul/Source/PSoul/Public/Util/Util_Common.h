// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"


struct FDamageInfo;
class AInventoryItemInstance;
struct FGameplayTag;

namespace Util_Common
{
	bool SpawnInventroyItemInstance(AActor* OwnerActor, TSubclassOf<AInventoryItemInstance> ItemClass);
	void PlayOpenPanelSound(UWorld* InWorld);
	void PlayClosePanelSound(UWorld* InWorld);
	void ApplyDamage(AActor* InCauser, AActor* InTarget, const FDamageInfo& DamageInfo);
	UTexture2D* GetBuffIconByBuffTag(FGameplayTag InBuffTag);
}
