// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillTreeNode.generated.h"

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
public:
	void UpdateNode(FName InSkillID);
	FName GetSkillID() const { return SkillID; }
	FVector2D GetNodePosition();
protected:
	void SetNodeUnlocked(bool bInUnlocked);

	
	UPROPERTY(meta = (BindWidget))
	UButton* SkillButton;
	
	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_Lock;
	
	FName SkillID;

	bool bUnlocked = false;
};
