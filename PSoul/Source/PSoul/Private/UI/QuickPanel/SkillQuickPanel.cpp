// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/QuickPanel/SkillQuickPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/QuickSkillManager.h"
#include "Development/Soul_UISetting.h"
#include "UI/QuickPanel/SkillQuickSlot.h"

void USkillQuickPanel::NativeConstruct()
{
	Super::NativeConstruct();

	HB_QuickSkillPanel->ClearChildren();

	if (UQuickSkillManager* QuickSkillManager = GetOwningPlayer()->FindComponentByClass<UQuickSkillManager>())
	{
		const TMap<FGameplayTag, FName>& QuickSkillSlots = QuickSkillManager->GetQuickSkillSlots();
		for (auto& pair : QuickSkillSlots)
		{
			if (USkillQuickSlot* SkillQuickSlot = CreateWidget<USkillQuickSlot>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->SkillQuickSlotClass.LoadSynchronous()))
			{
				SkillQuickSlot->InitSlot(pair.Key);
				HB_QuickSkillPanel->AddChildToHorizontalBox(SkillQuickSlot);
			}
		}
	}
}
