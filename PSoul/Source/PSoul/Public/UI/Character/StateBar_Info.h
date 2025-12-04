// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "StateBar_Info.generated.h"

struct FGameplayAttribute;
class UTextBlock;
class UStateBar;
/**
 * 
 */
UCLASS()
class PSOUL_API UStateBar_Info : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UStateBar* StateBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Name;

public:
	void Init(APawn* OwnerPawn, FGameplayAttribute InAttribute, FGameplayAttribute InMaxAttribute);
};
