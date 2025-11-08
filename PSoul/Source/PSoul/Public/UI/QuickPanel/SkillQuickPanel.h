// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillQuickPanel.generated.h"

class UHorizontalBox;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API USkillQuickPanel : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_QuickSkillPanel;
};
