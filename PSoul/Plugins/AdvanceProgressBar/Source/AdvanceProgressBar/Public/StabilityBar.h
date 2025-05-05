// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StabilityBar.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEPROGRESSBAR_API UStabilityBar : public UUserWidget
{
	GENERATED_BODY()

	
public:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void SetBarWidth(float InWidth);

	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void UpdateBar(float InTargetPercent);
	
public:
	//用来动态调整条的大小
	UPROPERTY(meta=(BindWidget))
	class USizeBox* BarSizeBox;
	
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* Bar;

private:
	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	FLinearColor BarColor=FLinearColor::Red;
	
};
