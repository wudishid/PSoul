// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "BuffBar.generated.h"

struct FGameplayTag;
class UHorizontalBox;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UBuffBar : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void HandleGameplayTagChanged(const FGameplayTag InChangedTag, int32 TagNumber);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_BuffBar;
};
