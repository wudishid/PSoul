// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillTreePanel.generated.h"

struct FOnAttributeChangeData;
class UTextBlock;
class USkillTreeManager;
class UHorizontalBox;

UCLASS(Abstract)
class PSOUL_API USkillTreePanel : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	void HandleLevelChanged(const FOnAttributeChangeData& ChangeData);
	
	void OnSkillLearned(TArray<FName> InLearnedSkillsID);
	
	TObjectPtr<USkillTreeManager> SkillTreeManagerComp = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_SkillTreePanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_SkillPoint;
	
};

