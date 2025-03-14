// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "StateBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PSOUL_API UStateBar : public USoulUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleAttributeChanged(FGameplayAttribute Attribute, float InCurrentValue, float InOldValue);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StateBar;

	UPROPERTY(EditAnywhere, Category = "StateBar")
	FLinearColor BarFillColor;
	
	UPROPERTY(EditAnywhere, Category = "StateBar")
	FName AttributeName;

	UPROPERTY(EditAnywhere, Category = "StateBar")
	FName MaxAttributeName;
	
	float CurrentValue = 0.f;
	float MaxValue = 0.f;
};
