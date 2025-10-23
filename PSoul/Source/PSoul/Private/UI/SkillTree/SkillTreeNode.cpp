// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillTree/SkillTreeNode.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Engine/AssetManager.h"
#include "SkillTreeSystem/SkillTreeData.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"

void USkillTreeNode::UpdateNode(FName InSkillID)
{
	SkillID = InSkillID;
	FSoftObjectPath path (TEXT("/Game/Data/SkillTree/Tree1/DA_SkillTree_Tree1.DA_SkillTree_Tree1"));
	FSoftObjectPtr SkillTreeDataPath(path);
	if (USkillTreeData* SkillTreeData = Cast<USkillTreeData>(SkillTreeDataPath.LoadSynchronous()))
	{
		if (USkillTreeNodeData* NodeData = SkillTreeData->GetSkillTreeNodeData(SkillID))
		{
			SkillImage->SetBrushFromTexture(NodeData->SkillIcon);
			SetNodeUnlocked(NodeData->bUnlocked);
		}
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
