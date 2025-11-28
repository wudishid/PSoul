// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/Buff/BuffSlot.h"

#include "GameplayTagContainer.h"
#include "Components/Image.h"
#include "Util/Util_Common.h"

void UBuffSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuffSlot::InitBuffSlot(FGameplayTag InBuffTag)
{
	if (UTexture2D* BuffIcon = Util_Common::GetBuffIconByBuffTag(InBuffTag))
	{
		BuffTag = InBuffTag;
		Image_BuffIcon->SetBrushFromTexture(BuffIcon);
	}
	else
	{
		RemoveFromParent();
	}
}
