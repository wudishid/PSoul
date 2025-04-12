// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UI/SoulUserWidget.h"
#include "AttributeRow.generated.h"

struct FOnAttributeChangeData;
class UHorizontalBox;
class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UAttributeRow : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void HandleAttributeChanged(const FOnAttributeChangeData& ChangeData);
	void HandleMaxAttributeChanged(const FOnAttributeChangeData& ChangeData);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_AttributeValue;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_AttributeMaxValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_AttributeMaxValue;
	
	UPROPERTY(EditAnywhere, Category = "AttributeRow")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, Category = "AttributeRow")
	bool HasMaxAttribute = false;
	
	UPROPERTY(EditAnywhere, Category = "AttributeRow", meta = (EditCondition = "HasMaxAttribute"))
	FGameplayAttribute MaxAttribute;
};
