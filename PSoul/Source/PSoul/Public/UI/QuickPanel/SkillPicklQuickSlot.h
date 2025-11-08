// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/SoulUserWidget.h"
#include "SkillPicklQuickSlot.generated.h"

class UImage;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API USkillPicklQuickSlot : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_SkillIcon;

private:
	void OnQuickSkillChanged(FGameplayTag InSkillInputTag);
	
public:
	void InitSlot(TPair<FGameplayTag, FName> InQuickSkillData, FName InTargetSkillID);
	void UpdateSlot();
protected:
	TPair<FGameplayTag, FName> QuickSkillData;
	FName TargetSkillID;
};
