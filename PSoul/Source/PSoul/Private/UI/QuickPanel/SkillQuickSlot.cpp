// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/QuickPanel/SkillQuickSlot.h"
#include "Components/Image.h"
#include "Components/QuickSkillManager.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"

void USkillQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillQuickSlot::InitSlot(FGameplayTag InSkillInputTag)
{
	SkillInputTag = InSkillInputTag;
	QuickSkillManager = GetOwningPlayer()->FindComponentByClass<UQuickSkillManager>();
	QuickSkillManager->OnQuickSkillChanged.AddUObject(this, &ThisClass::OnQuickSkillChanged);
	check(QuickSkillManager);
	SkillTreeManager = GetOwningPlayerPawn()->FindComponentByClass<USkillTreeManager>();
	check(SkillTreeManager);
}

void USkillQuickSlot::OnQuickSkillChanged(FGameplayTag InSkillInputTag)
{
	if (SkillInputTag == InSkillInputTag)
	{
		FName SkillID = *QuickSkillManager->GetQuickSkillSlots().Find(SkillInputTag);
		if (!SkillID.IsNone())
		{
			if (USkillTreeNodeData* NodeData = SkillTreeManager->GetSkillTreeNodeData(SkillID))
			{
				Image_SkillIcon->SetBrushFromTexture(NodeData->SkillIcon);
			}
		}
		else
		{
			Image_SkillIcon->SetBrushFromTexture(nullptr);
		}
	}
}
