// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillQuickPickPanel.generated.h"

class UWrapBox;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API USkillQuickPickPanel : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	void SetTargetSkillID(FName InTargetSkillID);
	void UpdatePanelPosition(FVector2D InPosition);
protected:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WB_SkillSlotsPanel;

	FName TargetSkillID;
};
