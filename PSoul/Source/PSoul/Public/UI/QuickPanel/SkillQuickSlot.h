// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/SoulUserWidget.h"
#include "SkillQuickSlot.generated.h"

class USkillTreeManager;
class UQuickSkillManager;
class UImage;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API USkillQuickSlot : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	void InitSlot(FGameplayTag InSkillInputTag);
private:
	void OnQuickSkillChanged(FGameplayTag InSkillInputTag);
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_SkillIcon;

	FGameplayTag SkillInputTag;

	UPROPERTY()
	TObjectPtr<UQuickSkillManager> QuickSkillManager;

	UPROPERTY()
	TObjectPtr<USkillTreeManager> SkillTreeManager;
};
