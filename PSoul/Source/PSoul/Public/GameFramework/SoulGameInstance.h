// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoulGameInstance.generated.h"

enum class ESoulCharacterTeam : uint8;
/**
 * 
 */
UCLASS()
class PSOUL_API USoulGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
public:
	FString EnterGameMap;
	FName PlayerName;
};
