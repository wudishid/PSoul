// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerController.h"
#include "SoulPlayerController_Login.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API ASoulPlayerController_Login : public ASoulPlayerController
{
	GENERATED_BODY()
protected:
	virtual void AcknowledgePossession(class APawn* P) override;
};
