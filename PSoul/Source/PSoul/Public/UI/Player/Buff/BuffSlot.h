// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/SoulUserWidget.h"
#include "BuffSlot.generated.h"

class UImage;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UBuffSlot : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	void InitBuffSlot(FGameplayTag InBuffTag);
	FGameplayTag GetBuffTag() const { return BuffTag; }
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_BuffIcon;

	FGameplayTag BuffTag;
};
