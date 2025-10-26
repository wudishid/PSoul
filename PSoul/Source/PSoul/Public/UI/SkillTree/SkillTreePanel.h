// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillTreePanel.generated.h"

class USkillTreeManager;
class UHorizontalBox;

UCLASS()
class PSOUL_API USkillTreePanel : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	TObjectPtr<USkillTreeManager> SkillTreeManagerComp = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_SkillTreePanel;
};
