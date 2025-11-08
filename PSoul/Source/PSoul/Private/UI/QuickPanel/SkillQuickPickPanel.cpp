// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/QuickPanel/SkillQuickPickPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/QuickSkillManager.h"
#include "Components/WrapBox.h"
#include "Development/Soul_UISetting.h"
#include "UI/QuickPanel/SkillPicklQuickSlot.h"

void USkillQuickPickPanel::NativeConstruct()
{
	Super::NativeConstruct();

	WB_SkillSlotsPanel->ClearChildren();
	if (UQuickSkillManager* QuickSkillManager = GetOwningPlayer()->FindComponentByClass<UQuickSkillManager>())
	{
		const TMap<FGameplayTag, FName>& QuickSkillSlots = QuickSkillManager->GetQuickSkillSlots();
		for (auto& pair : QuickSkillSlots)
		{
			if (USkillPicklQuickSlot* SkillPicklQuickSlot = CreateWidget<USkillPicklQuickSlot>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->SkillPicklQuickSlotClass.LoadSynchronous()))
			{
				SkillPicklQuickSlot->InitSlot(pair, TargetSkillID);
				WB_SkillSlotsPanel->AddChildToWrapBox(SkillPicklQuickSlot);
			}
		}
	}
}

FReply USkillQuickPickPanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		RemoveFromParent();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void USkillQuickPickPanel::SetTargetSkillID(FName InTargetSkillID)
{
	TargetSkillID = InTargetSkillID;
}

void USkillQuickPickPanel::UpdatePanelPosition(FVector2D InPosition)
{
	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(WB_SkillSlotsPanel->Slot))
	{
		CanvasPanelSlot->SetPosition(InPosition);
	}
}
