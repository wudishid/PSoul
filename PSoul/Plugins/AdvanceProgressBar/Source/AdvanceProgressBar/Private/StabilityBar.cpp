// Fill out your copyright notice in the Description page of Project Settings.


#include "StabilityBar.h"

#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UStabilityBar::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(Bar)
	{
		FProgressBarStyle BarStyle=Bar->GetWidgetStyle();
		BarStyle.BackgroundImage.TintColor=FSlateColor(FColor(0,0,0,0));
		Bar->SetPercent(0);
		Bar->SetFillColorAndOpacity(BarColor);
		Bar->SetWidgetStyle(BarStyle);
		Bar->SetBarFillType(EProgressBarFillType::FillFromCenterHorizontal);
	}
}

void UStabilityBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStabilityBar::SetBarWidth(float InWidth)
{
	BarSizeBox->SetWidthOverride(InWidth);
}

void UStabilityBar::UpdateBar(float InTargetPercent)
{
	if(InTargetPercent<=0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
		Bar->SetPercent(InTargetPercent);
	}
}
