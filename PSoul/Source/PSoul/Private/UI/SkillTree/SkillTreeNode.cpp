// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SkillTree/SkillTreeNode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"


void USkillTreeNode::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeNode::InitNode(FName InSkillID)
{
	SkillID = InSkillID;

	SkillTreeManagerComp = GetOwningPlayerPawn()->FindComponentByClass<USkillTreeManager>();
	checkf(SkillTreeManagerComp, TEXT("SkillTreeManagerComp Is not valid!"));
	SkillTreeManagerComp->OnSkillUnlocked.AddUObject(this, &ThisClass::OnSkillUnlocked);
	
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

void USkillTreeNode::OnSkillBtnClicked()
{
	if (bUnlocked)
	{
		SkillTreeManagerComp->TryLearnSkill(SkillID);
	}
}

void USkillTreeNode::OnSkillUnlocked(TArray<FName> InUnlockedSkillsID)
{
	if (InUnlockedSkillsID.Contains(SkillID))
	{
		SetNodeUnlocked(true);
	}
}
