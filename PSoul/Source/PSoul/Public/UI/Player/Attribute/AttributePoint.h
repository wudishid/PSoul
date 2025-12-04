// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UI/SoulUserWidget.h"
#include "AttributePoint.generated.h"

class USoulAbilitySystemComponent;
struct FOnAttributeChangeData;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UAttributePoint : public USoulUserWidget
{
	GENERATED_BODY()
public:
	void Init();
protected:
	virtual void NativeConstruct() override;

	void HandleAttributeChanged(const FOnAttributeChangeData& ChangeData);

	UFUNCTION()
	void HandleBtn_AddPointClicked();
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_AddPoint;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_AttributePointValue;
	
	UPROPERTY(EditAnywhere, Category = "AttributePoint")
	FGameplayAttribute Attribute;

	UPROPERTY()
	USoulAbilitySystemComponent* ASC = nullptr;
};
