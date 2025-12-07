// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SoulUserWidget.h"

#include "Development/Soul_UISetting.h"

void USoulUserWidget::ShowInViewport()
{
	if (!IsInViewport())
	{
		AddToViewport(GetZOrder());
	}
}

int32 USoulUserWidget::GetZOrder() const
{
	if (GetDefault<USoul_UISetting>()->WidgetsZOrderMap.Contains(GetClass()))
	{
		return *GetDefault<USoul_UISetting>()->WidgetsZOrderMap.Find(GetClass());
	}

	return 0;
}
