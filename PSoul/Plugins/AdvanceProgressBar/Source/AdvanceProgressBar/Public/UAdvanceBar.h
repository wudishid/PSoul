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
public:
	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void SetBarWidth(float InWidth);

	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void UpdateBar(float InTargetPercent,EAdvanceBarUpdateStyle UpdateStyle=EAdvanceBarUpdateStyle::DirectSet);

	float GetBarPercent() const;

	UFUNCTION(BlueprintCallable,Category="AdvanceBar")
	void SetBarColor(FLinearColor InFrontColor, FLinearColor InBackColor);

	virtual void BeginDestroy() override;
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
	
	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	FLinearColor FrontBarColor=FLinearColor::Red;;

	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	FLinearColor BackBarColor=FLinearColor::Yellow;

	//直接更新条的百分比所花费的时间
	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	float DirectSetBarPercentTimeLength=0.2f;

	//背景条跟随插值到前景条花费的时间
	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	float LerpSetBarPercentTimeLength=0.5f;

	//更新频率，越大越顺滑，但会耗费更多性能
	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	float UpdateFrequency=60;
private:
	void ClearAllBarTimers();
private:
	UPROPERTY()
	FTimerHandle FrontBarLerpTimerHandle;

	UPROPERTY()
	FTimerHandle BackBarLerpTimerHandle;

	//背景条延迟一段时间后跟随到前景条百分比
	UPROPERTY()
	FTimerHandle BackBarLerpDelayTimerHandle;

	

	float CurrentFrontBarPercent;
	float CurrentBackBarPercent;
	float FrontBarLerpAlpha;
	float BackBarLerpAlpha;
	float TargetPercent;
};
