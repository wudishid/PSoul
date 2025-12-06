// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillTreeNode.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class USkillTreeManager;
class UBorder;
class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class PSOUL_API USkillTreeNode : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	void InitNode(FName InSkillID);
	void UpdateNode();
	FName GetSkillID() const { return SkillID; }
	FVector2D GetNodePosition();
protected:
	void SetNodeUnlocked(bool bInUnlocked);
	void SetNodeLearned(bool bInLearned);
	
	UFUNCTION()
	void OnSkillBtnClicked();

	void OnSkillUnlocked(TArray<FName> InUnlockedSkillsID);
	void OnSkillLearned(TArray<FName> InLearnedSkillsID);

	UWidget* OnGetTooltipWidget();
	
	UPROPERTY(meta = (BindWidget))
	UButton* SkillButton;
	
	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_UnLearnedMask;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_Lock;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WS_SkillTypeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SkillPointCost;
	
	UPROPERTY()
	TObjectPtr<USkillTreeManager> SkillTreeManagerComp;
	
	FName SkillID;

	bool bUnlocked = false;
	bool bLearned = false;
};
