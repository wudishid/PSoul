// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillTreeNodeLine.generated.h"

class USizeBox;
class UImage;
/**
 * 
 */
UCLASS()
class PSOUL_API USkillTreeNodeLine : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SB_Line;
public:
	void SetLineLength(float InLength);
};
