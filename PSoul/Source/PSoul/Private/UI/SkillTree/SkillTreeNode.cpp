// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SkillTree/SkillTreeNode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/KismetSystemLibrary.h"
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
	if (bInLearned)
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
	if (bUnlocked)
	{
		if (!SkillTreeManagerComp->TryLearnSkill(SkillID))
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SkillPoints not enough!"));
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
