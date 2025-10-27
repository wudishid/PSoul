// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillInfoTip.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PSOUL_API USkillInfoTip : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void InitTip(FText InTip);
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Tip;
};
