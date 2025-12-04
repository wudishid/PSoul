// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "AttributePanel.generated.h"

class UAttributePoint;
class UAttributeRow;
class UTextBlock;
class UVerticalBox;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UAttributePanel : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void Init();
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_NickName;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_Level;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_NextLevelNeedSoul;

	UPROPERTY(meta = (BindWidget))
	UAttributePoint* AttributePoint_Life;

	UPROPERTY(meta = (BindWidget))
	UAttributePoint* AttributePoint_Strength;

	UPROPERTY(meta = (BindWidget))
	UAttributePoint* AttributePoint_Stamina;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_Health;
	
	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_Stamina;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_Strength;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_PhysicalDefence;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_MagicDefence;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_MaxSpeed;

	UPROPERTY(meta = (BindWidget))
	UAttributeRow* AttributeRow_Soul;
	
};
