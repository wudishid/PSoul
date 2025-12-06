// Fill out your copyright notice in the Description page of Project Settings.
#include "UAdvanceBar.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"


void UAdvanceBar::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetBarWidth(BarWidth);
	SetBarColor(FrontBarColor, BackBarColor);
}

void UAdvanceBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bUpdateBar)
	{
		if (UpdateStyle == EAdvanceBarUpdateStyle::DirectSet)
		{
			FrontBar->SetPercent(TargetPercent);

			BackBar->SetPercent(TargetPercent);

			bUpdateBar = false;
		}
		else
		{
			FrontBarLerpAlpha += InDeltaTime / DirectSetBarPercentTimeLength;
			if (FrontBarLerpAlpha <= 1)
			{
				FrontBar->SetPercent(
					FMath::Lerp(CurrentFrontBarPercent, TargetPercent, FrontBarLerpAlpha));
			}
			
			
			if (bUpdateBackBar)
			{
				BackBarLerpAlpha += InDeltaTime / LerpSetBarPercentTimeLength;
				if (BackBarLerpAlpha <= 1)
				{
					BackBar->SetPercent(
						FMath::Lerp(CurrentBackBarPercent, TargetPercent, BackBarLerpAlpha));
				}
				else
				{
					ResetParameter();
				}
			}
			else
			{
				BackBarDelayTime += InDeltaTime;
				if (BackBarDelayTime >= 0.5f)
				{
					BackBarLerpAlpha += InDeltaTime / LerpSetBarPercentTimeLength;
					if (BackBarLerpAlpha <= 1)
					{
						BackBar->SetPercent(
						FMath::Lerp(CurrentBackBarPercent, TargetPercent, BackBarLerpAlpha));
					}
					else
					{
						ResetParameter();
					}
				}
			}
		}
	}
}

void UAdvanceBar::SetBarWidth(float InWidth)
{
	BarSizeBox->SetWidthOverride(InWidth);
}

void UAdvanceBar::UpdateBar(float InTargetPercent, EAdvanceBarUpdateStyle InUpdateStyle)
{
	TargetPercent = InTargetPercent;
	UpdateStyle = InUpdateStyle;
	CurrentFrontBarPercent = FrontBar->GetPercent();
	CurrentBackBarPercent = BackBar->GetPercent();
	FrontBarLerpAlpha = 0;
	BackBarLerpAlpha = 0;

	//如果与当前的条百分比相同，则返回
	if (CurrentFrontBarPercent == InTargetPercent) { return; }

	//如果比当前的条百分比大，则设置更新条方式为直接更新
	if (InTargetPercent > CurrentFrontBarPercent) { UpdateStyle = EAdvanceBarUpdateStyle::DirectSet; }

	if (bUpdateBar)
	{
		if (!bUpdateBackBar)
		{
			bUpdateBackBar = true;
		}
	}
	
	bUpdateBar = true;
}

float UAdvanceBar::GetBarPercent() const
{
	return FrontBar->GetPercent();
}

void UAdvanceBar::SetBarColor(FLinearColor InFrontColor, FLinearColor InBackColor)
{
	FrontBar->SetFillColorAndOpacity(InFrontColor);
	BackBar->SetFillColorAndOpacity(InBackColor);
}

void UAdvanceBar::ResetParameter()
{
	CurrentFrontBarPercent = 0.f;
	CurrentBackBarPercent = 0.f;
	FrontBarLerpAlpha = 0.f;
	BackBarLerpAlpha = 0.f;
	TargetPercent = 0.f;
	BackBarDelayTime = 0.f;
	bUpdateBar = false;
	bUpdateBackBar = false;
}



