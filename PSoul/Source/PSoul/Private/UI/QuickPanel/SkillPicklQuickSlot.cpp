// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/QuickPanel/SkillPicklQuickSlot.h"
#include "Components/Image.h"
#include "Components/QuickSkillManager.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"

void USkillPicklQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply USkillPicklQuickSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (UQuickSkillManager* QuickSkillManager = GetOwningPlayer()->FindComponentByClass<UQuickSkillManager>())
		{
			QuickSkillManager->SetQuickSkill(QuickSkillData.Key, TargetSkillID);
			return FReply::Handled();
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


void USkillPicklQuickSlot::OnQuickSkillChanged(FGameplayTag InSkillInputTag)
{
	if (QuickSkillData.Key == InSkillInputTag)
	{
		if (UQuickSkillManager* QuickSkillManager = GetOwningPlayer()->FindComponentByClass<UQuickSkillManager>())
		{
			QuickSkillData.Value = *QuickSkillManager->GetQuickSkillSlots().Find(QuickSkillData.Key);
			UpdateSlot();
		}
	}
}

void USkillPicklQuickSlot::InitSlot(TPair<FGameplayTag, FName> InQuickSkillData, FName InTargetSkillID)
{
	QuickSkillData = InQuickSkillData;
	TargetSkillID = InTargetSkillID;
	if (UQuickSkillManager* QuickSkillManager = GetOwningPlayer()->FindComponentByClass<UQuickSkillManager>())
	{
		QuickSkillManager->OnQuickSkillChanged.AddUObject(this, &ThisClass::OnQuickSkillChanged);
	}
	UpdateSlot();
}

void USkillPicklQuickSlot::UpdateSlot()
{
	if (!QuickSkillData.Value.IsNone())
	{
		if (USkillTreeManager* SkillTreeManager = GetOwningPlayerPawn()->FindComponentByClass<USkillTreeManager>())
		{
			if (USkillTreeNodeData* NodeData = SkillTreeManager->GetSkillTreeNodeData(QuickSkillData.Value))
			{
				Image_SkillIcon->SetBrushFromTexture(NodeData->SkillIcon);
			}
		}
	}
	else
	{
		Image_SkillIcon->SetBrushFromTexture(nullptr);
	}
}
