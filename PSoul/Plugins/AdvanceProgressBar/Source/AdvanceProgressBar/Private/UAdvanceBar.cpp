// Fill out your copyright notice in the Description page of Project Settings.


#include "UAdvanceBar.h"

#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"


#define TimerManager GetWorld()->GetTimerManager()


void UAdvanceBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	//前景条的背景透明度设为0
	FProgressBarStyle FrontBarStyle=FrontBar->GetWidgetStyle();
	FrontBarStyle.BackgroundImage.TintColor=FSlateColor(FColor(0,0,0,0));
	FrontBar->SetPercent(1);
	FrontBar->SetFillColorAndOpacity(FrontBarColor);
	FrontBar->SetWidgetStyle(FrontBarStyle);
	BackBar->SetPercent(1);
	BackBar->SetFillColorAndOpacity(BackBarColor);
}

void UAdvanceBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAdvanceBar::NativeDestruct()
{
	Super::NativeDestruct();

	ClearAllBarTimers();
}

void UAdvanceBar::SetBarWidth(float InWidth)
{
	BarSizeBox->SetWidthOverride(InWidth);
}

void UAdvanceBar::UpdateBar(float InTargetPercent, EAdvanceBarUpdateStyle UpdateStyle)
{
	TargetPercent=InTargetPercent;
	CurrentFrontBarPercent=FrontBar->GetPercent();
	CurrentBackBarPercent=BackBar->GetPercent();
	FrontBarLerpAlpha=0;
	BackBarLerpAlpha=0;
	
	//如果与当前的条百分比相同，则返回
	if(CurrentFrontBarPercent==InTargetPercent){return;}

	//如果比当前的条百分比大，则设置更新条方式为直接更新
	if(InTargetPercent>CurrentFrontBarPercent){UpdateStyle=EAdvanceBarUpdateStyle::DirectSet;}
	
	switch (UpdateStyle)
	{
	//如果更新方式为直接设置，则前景条和背景条同时到目标百分比
	case EAdvanceBarUpdateStyle::DirectSet:
		//移除所有条计时器
		ClearAllBarTimers();
		FrontBar->SetPercent(InTargetPercent);
		BackBar->SetPercent(InTargetPercent);
		break;

	//插值到目标百分比
	case EAdvanceBarUpdateStyle::Lerp:
		
		TimerManager.SetTimer(FrontBarLerpTimerHandle,FTimerDelegate::CreateLambda
				([&]()
				{
					FrontBarLerpAlpha+=(1/UpdateFrequency)/DirectSetBarPercentTimeLength;
					if (FrontBarLerpAlpha>=1)
					{
						FrontBarLerpAlpha=1;
						TimerManager.ClearTimer(FrontBarLerpTimerHandle);
					}
					FrontBar->SetPercent(FMath::Lerp(CurrentFrontBarPercent,TargetPercent,FrontBarLerpAlpha));
				}
				),1/UpdateFrequency,true);


		//如果背景条等待计时器活跃，则让背景条不等待直接插值
		if(BackBarLerpDelayTimerHandle.IsValid())
		{
			//移除背景条等待计时器
			TimerManager.ClearTimer(BackBarLerpDelayTimerHandle);

			//背景条插值
			TimerManager.SetTimer(BackBarLerpTimerHandle,FTimerDelegate::CreateLambda
						([&]()
						{
							BackBarLerpAlpha+=(1/UpdateFrequency)/LerpSetBarPercentTimeLength;
							if (BackBarLerpAlpha>=1)
							{
								BackBarLerpAlpha=1;
								TimerManager.ClearTimer(BackBarLerpTimerHandle);
							}
							BackBar->SetPercent(FMath::Lerp(CurrentBackBarPercent,TargetPercent,BackBarLerpAlpha));
						}
						),1/UpdateFrequency,true);
		}
		else
		{
			//等待一会后，背景条插值到目标百分比
			TimerManager.SetTimer(BackBarLerpDelayTimerHandle,FTimerDelegate::CreateLambda
				([&]()
				{
					
					TimerManager.SetTimer(BackBarLerpTimerHandle,FTimerDelegate::CreateLambda
						([&]()
						{
							BackBarLerpAlpha+=(1/UpdateFrequency)/LerpSetBarPercentTimeLength;
							if (BackBarLerpAlpha>=1)
							{
								BackBarLerpAlpha=1;
								TimerManager.ClearTimer(BackBarLerpTimerHandle);
							}
							BackBar->SetPercent(FMath::Lerp(CurrentBackBarPercent,TargetPercent,BackBarLerpAlpha));
						}
						),1/UpdateFrequency,true);
					
					TimerManager.ClearTimer(BackBarLerpDelayTimerHandle);
				}
				),0.5f,false);
		}
		break;
	default:
		break;
	}
}

float UAdvanceBar::GetBarPercent() const
{
	return FrontBar->GetPercent();
}

void UAdvanceBar::ClearAllBarTimers()
{
	if(FrontBarLerpTimerHandle.IsValid())
	{
		TimerManager.ClearTimer(FrontBarLerpTimerHandle);
	}
	if(BackBarLerpTimerHandle.IsValid())
	{
		TimerManager.ClearTimer(BackBarLerpTimerHandle);
	}
	if(BackBarLerpDelayTimerHandle.IsValid())
	{
		TimerManager.ClearTimer(BackBarLerpDelayTimerHandle);
	}
}
