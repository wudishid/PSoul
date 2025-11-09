// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/SoulUserWidget.h"
#include "SkillQuickSlot.generated.h"

class UTextBlock;
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
	void OnQuickSkillReleased(FGameplayTag InSkillInputTag);
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_SkillIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_CD;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_CD;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SkillKey;
	
	FGameplayTag SkillInputTag;

	UPROPERTY()
	TObjectPtr<UQuickSkillManager> QuickSkillManager;

	UPROPERTY()
	TObjectPtr<USkillTreeManager> SkillTreeManager;

private:
	float CurrentTime = 0.f;
	float SkillCDTime = 0.1f;
	
	FTimerHandle Timer_CD;
};
