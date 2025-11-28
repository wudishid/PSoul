// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "Soul_CommonSetting.generated.h"

/**
 * 
 */
UCLASS(config = Game, DefaultConfig)
class PSOUL_API USoul_CommonSetting : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Config, Category = "Soul Common Settings | Sound")
	TSoftObjectPtr<USoundBase> PickItemSound;

	UPROPERTY(EditAnywhere, Config, Category = "Soul Common Settings | Sound")
	TSoftObjectPtr<USoundBase> DropItemSound;
	
	UPROPERTY(EditAnywhere, Config, Category = "Soul Common Settings | Sound")
	TSoftObjectPtr<USoundBase> OpenPanelSound;

	UPROPERTY(EditAnywhere, Config, Category = "Soul Common Settings | Sound")
	TSoftObjectPtr<USoundBase> ClosePanelSound;


	UPROPERTY(EditAnywhere, Config, Category = "Soul Common Settings | Buff")
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> BuffsIconMap;
};
