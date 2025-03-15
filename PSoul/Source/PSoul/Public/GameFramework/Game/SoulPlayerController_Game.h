// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerController.h"
#include "SoulPlayerController_Game.generated.h"

class USoulAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PSOUL_API ASoulPlayerController_Game : public ASoulPlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	USoulAbilitySystemComponent* GetAbilitySystemComponent() const;
	
};
