// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UAdvanceBar.generated.h"


class UProgressBar;
class USizeBox;

UENUM(BlueprintType)
enum class EAdvanceBarUpdateStyle:uint8
{
	//直接设置条的长度
	DirectSet,

	//插值到目标长度
	Lerp
};


/**
 * 
 */
UCLASS(Blueprintable)
class ADVANCEPROGRESSBAR_API UAdvanceBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void SetBarWidth(float InWidth);

	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void UpdateBar(float InTargetPercent, EAdvanceBarUpdateStyle InUpdateStyle = EAdvanceBarUpdateStyle::DirectSet);

	float GetBarPercent() const;

	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void SetBarColor(FLinearColor InFrontColor, FLinearColor InBackColor);
protected:
	//用来动态调整条的大小
	UPROPERTY(meta=(BindWidget))
	USizeBox* BarSizeBox;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* FrontBar;

	//背景条，用来显示变化了多少
	UPROPERTY(meta=(BindWidget))
	UProgressBar* BackBar;

	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	float BarWidth = 150.f;

	UPROPERTY(EditAnywhere, Category="AdvanceBar")
	FLinearColor FrontBarColor = FLinearColor::Red;;

	UPROPERTY(EditAnywhere, Category="AdvanceBar")
	FLinearColor BackBarColor = FLinearColor::Yellow;

	//直接更新条的百分比所花费的时间
	UPROPERTY(EditAnywhere, Category="AdvanceBar")
	float DirectSetBarPercentTimeLength = 0.2f;

	//背景条跟随插值到前景条花费的时间
	UPROPERTY(EditAnywhere, Category="AdvanceBar")
	float LerpSetBarPercentTimeLength = 0.5f;

private:

	void ResetParameter();
	
	EAdvanceBarUpdateStyle UpdateStyle;
	float CurrentFrontBarPercent = 0.f;
	float CurrentBackBarPercent = 0.f;
	float FrontBarLerpAlpha = 0.f;
	float BackBarLerpAlpha = 0.f;
	float TargetPercent = 0.f;
	float BackBarDelayTime = 0.f;
	bool bUpdateBar = false;
	bool bUpdateBackBar = false;
};
