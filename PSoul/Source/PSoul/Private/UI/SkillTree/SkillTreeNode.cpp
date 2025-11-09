// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SkillTree/SkillTreeNode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Development/Soul_UISetting.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"
#include "UI/QuickPanel/SkillQuickPanel.h"
#include "UI/QuickPanel/SkillQuickPickPanel.h"
#include "UI/SkillTree/SkillInfoTip.h"


void USkillTreeNode::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeNode::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (USkillInfoTip* CurrentTip = Cast<USkillInfoTip>(GetToolTip()))
	{
		CurrentTip->InitTip(SkillTreeManagerComp->GetSkillTreeNodeData(SkillID)->SkillDescription);
	}
	else
	{
		if (USkillInfoTip* Tip = CreateWidget<USkillInfoTip>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->SkillInfoTipClass.LoadSynchronous()))
		{
			Tip->InitTip(SkillTreeManagerComp->GetSkillTreeNodeData(SkillID)->SkillDescription);
			SetToolTip(Tip);
		}
	}
}

void USkillTreeNode::InitNode(FName InSkillID)
{
	SkillID = InSkillID;

	SkillTreeManagerComp = GetOwningPlayerPawn()->FindComponentByClass<USkillTreeManager>();
	checkf(SkillTreeManagerComp, TEXT("SkillTreeManagerComp Is not valid!"));
	SkillTreeManagerComp->OnSkillUnlocked.AddUObject(this, &ThisClass::OnSkillUnlocked);
	SkillTreeManagerComp->OnSkillLearned.AddUObject(this, &ThisClass::OnSkillLearned);
	
	SkillButton->OnClicked.AddDynamic(this, &ThisClass::OnSkillBtnClicked);
	
	UpdateNode();
}

void USkillTreeNode::UpdateNode()
{
	if (USkillTreeNodeData* NodeData = SkillTreeManagerComp->GetSkillTreeNodeData(SkillID))
	{
		SkillImage->SetBrushFromTexture(NodeData->SkillIcon);
		SetNodeUnlocked(SkillTreeManagerComp->IsSkillUnlocked(SkillID));
	}
}

FVector2D USkillTreeNode::GetNodePosition()
{
	if (UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this))
	{
		return CanvasPanelSlot->GetPosition();
	}
	return FVector2D();
}

void USkillTreeNode::SetNodeUnlocked(bool bInUnlocked)
{
	bUnlocked = bInUnlocked;
	if (bUnlocked)
	{
		Border_Lock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Border_Lock->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillTreeNode::SetNodeLearned(bool bInLearned)
{
	bLearned = bInLearned;
	if (bLearned)
	{
		Image_UnLearnedMask->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Image_UnLearnedMask->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillTreeNode::OnSkillBtnClicked()
{
	if (bUnlocked && !bLearned)
	{
		if (!SkillTreeManagerComp->TryLearnSkill(SkillID))
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SkillPoints not enough!"));
		}
	}
	//创建快捷技能设置面板
	else if (bUnlocked && bLearned)
	{
		if (SkillTreeManagerComp->CanSkillRelease(SkillID))
		{
			if (USkillQuickPickPanel* SkillQuickPanel = CreateWidget<USkillQuickPickPanel>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->SkillQuickPickPanelClass.LoadSynchronous()))
			{
				SkillQuickPanel->SetTargetSkillID(SkillID);
				SkillQuickPanel->AddToViewport();
				FVector2D Pos;
				UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), Pos.X, Pos.Y);
				SkillQuickPanel->UpdatePanelPosition(Pos);
			}
		}
	}
}

void USkillTreeNode::OnSkillUnlocked(TArray<FName> InUnlockedSkillsID)
{
	if (InUnlockedSkillsID.Contains(SkillID))
	{
		SetNodeUnlocked(true);
	}
}

void USkillTreeNode::OnSkillLearned(TArray<FName> InLearnedSkillsID)
{
	if (InLearnedSkillsID.Contains(SkillID))
	{
		SetNodeLearned(true);
	}
}

UWidget* USkillTreeNode::OnGetTooltipWidget()
{
	if (USkillInfoTip* Tip = CreateWidget<USkillInfoTip>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->SkillInfoTipClass.LoadSynchronous()))
	{
		Tip->InitTip(SkillTreeManagerComp->GetSkillTreeNodeData(SkillID)->SkillDescription);
		return Tip;
	}
	return nullptr;
}
