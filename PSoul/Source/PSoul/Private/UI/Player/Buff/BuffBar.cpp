// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/Buff/BuffBar.h"

#include "Components/HorizontalBox.h"
#include "Development/Soul_UISetting.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "PSoul/SoulGameplayTags.h"
#include "UI/Player/Buff/BuffSlot.h"

void UBuffBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (USoulAbilitySystemComponent* ASC = GetOwningPlayerPawn()->FindComponentByClass<USoulAbilitySystemComponent>())
	{
		if (!ASC->RegisterGenericGameplayTagEvent().IsBoundToObject(this))
		{
			ASC->RegisterGenericGameplayTagEvent().AddUObject(this, &ThisClass::HandleGameplayTagChanged);
		}
	}
}

void UBuffBar::HandleGameplayTagChanged(const FGameplayTag InChangedTag, int32 TagNumber)
{
	if (InChangedTag.MatchesAny(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Buff.Gain")))))
	{
		if (TagNumber > 0)
		{
			if (UBuffSlot* BuffSlot = CreateWidget<UBuffSlot>(GetOwningPlayer(),
														  GetDefault<USoul_UISetting>()->BuffSlotClass.
														  LoadSynchronous()))
			{
				HB_BuffBar->AddChildToHorizontalBox(BuffSlot);
				BuffSlot->InitBuffSlot(InChangedTag);
			}
		}
		else
		{
			for (UWidget* Buff : HB_BuffBar->GetAllChildren())
			{
				if (UBuffSlot* BuffSlot = Cast<UBuffSlot>(Buff))
				{
					if (BuffSlot->GetBuffTag() == InChangedTag)
					{
						HB_BuffBar->RemoveChild(BuffSlot);
						break;
					}
				}
			}
		}
	}
}
