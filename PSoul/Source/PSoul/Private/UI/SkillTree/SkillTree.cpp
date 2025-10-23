// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SkillTree/SkillTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Development/Soul_UISetting.h"
#include "SkillTreeSystem/SkillTreeData.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"
#include "UI/SkillTree/SkillTreeNode.h"
#include "UI/SkillTree/SkillTreeNodeLine.h"

void USkillTree::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	BuildSkillTree();
}

void USkillTree::BuildSkillTree()
{
	if (!SkillTreeData) return;
	
	CP_SkillTree->ClearChildren();

	USkillTreeNode* RootNode = CreateSkillTreeNode(SkillTreeData->RootNodeData->SkillID, FVector2D(250, 0));
	CreateChildNodes(RootNode);
}

USkillTreeNode* USkillTree::CreateSkillTreeNode(FName InSkillID, FVector2D InNodePosition)
{
	if (USkillTreeNode* TreeNode = CreateWidget<USkillTreeNode>(GetWorld(), GetDefault<USoul_UISetting>()->SkillTreeNodeClass.LoadSynchronous()))
	{
		TreeNode->UpdateNode(InSkillID);
		CP_SkillTree->AddChildToCanvas(TreeNode);
		if (UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TreeNode))
		{
			CanvasPanelSlot->SetAutoSize(true);
			CanvasPanelSlot->SetAlignment(FVector2D(0.5, 0));
			CanvasPanelSlot->SetPosition(InNodePosition);
		}
		return TreeNode;
	}
	
	return nullptr;
}

USkillTreeNodeLine* USkillTree::CreateSkillTreeNodeLine(FVector2D InNodePosition)
{
	if (USkillTreeNodeLine* NodeLine = CreateWidget<USkillTreeNodeLine>(GetWorld(), GetDefault<USoul_UISetting>()->SkillTreeNodeLineClass.LoadSynchronous()))
	{
		CP_SkillTree->AddChildToCanvas(NodeLine);
		if (UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(NodeLine))
		{
			CanvasPanelSlot->SetAutoSize(true);
			CanvasPanelSlot->SetAlignment(FVector2D(0.5, 0));
			CanvasPanelSlot->SetPosition(FVector2D(InNodePosition.X, InNodePosition.Y));
		}
		return NodeLine;
	}
	
	return nullptr;
}

void USkillTree::CreateChildNodes(USkillTreeNode* Parent)
{
	if (!Parent) return;
	
	if (FSkillTreeNodes* TreeNodes = SkillTreeData->SkillNodeDatas.Find(Parent->GetSkillID()))
	{
		if (TreeNodes->SkillTreeNodes.Num() == 1)
		{
			USkillTreeNode* ChildNode = nullptr;
			CreateOneChildNode(Parent, TreeNodes->SkillTreeNodes[0]->SkillID, ChildNode);
			CreateChildNodes(ChildNode);
		}
		else if (TreeNodes->SkillTreeNodes.Num() == 2)
		{
			USkillTreeNode* ChildNode1 = nullptr;
			USkillTreeNode* ChildNode2 = nullptr;
			CreateTwoChildNode(Parent, TreeNodes->SkillTreeNodes[0]->SkillID, TreeNodes->SkillTreeNodes[1]->SkillID,
			                   ChildNode1, ChildNode2);
			CreateChildNodes(ChildNode1);
			CreateChildNodes(ChildNode2);
		}
		else if (TreeNodes->SkillTreeNodes.Num() == 3)
		{
			USkillTreeNode* ChildNode1 = nullptr;
			USkillTreeNode* ChildNode2 = nullptr;
			USkillTreeNode* ChildNode3 = nullptr;
			CreateThreeChildNode(Parent, TreeNodes->SkillTreeNodes[0]->SkillID, TreeNodes->SkillTreeNodes[1]->SkillID,
							   TreeNodes->SkillTreeNodes[2]->SkillID, ChildNode1, ChildNode2, ChildNode3);
			CreateChildNodes(ChildNode1);
			CreateChildNodes(ChildNode2);
			CreateChildNodes(ChildNode3);
		}
	}
}

void USkillTree::CreateOneChildNode(USkillTreeNode* Parent, FName ChildNodeSkillID, USkillTreeNode*& OutChildNode)
{
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X, Parent->GetNodePosition().Y + 80));
	OutChildNode = CreateSkillTreeNode(ChildNodeSkillID, FVector2D(Parent->GetNodePosition().X, Parent->GetNodePosition().Y + 160));
}

void USkillTree::CreateTwoChildNode(USkillTreeNode* Parent, FName ChildNode1SkillID, FName ChildNode2SkillID, USkillTreeNode*& OutChildNode1,
	USkillTreeNode*& OutChildNode2)
{
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X, Parent->GetNodePosition().Y + 80));

	if (USkillTreeNodeLine* NodeLine = CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X - 10, Parent->GetNodePosition().Y + 160)))
	{
		NodeLine->SetRenderTransformPivot(FVector2D(1,0));
		NodeLine->SetRenderTransformAngle(90);
	}
	if (USkillTreeNodeLine* NodeLine = CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X + 10, Parent->GetNodePosition().Y + 160)))
	{
		NodeLine->SetRenderTransformPivot(FVector2D(0,0));
		NodeLine->SetRenderTransformAngle(-90);
	}
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X - 90, Parent->GetNodePosition().Y + 150));
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X + 90, Parent->GetNodePosition().Y + 150));

	OutChildNode1 = CreateSkillTreeNode(ChildNode1SkillID, FVector2D(Parent->GetNodePosition().X - 90, Parent->GetNodePosition().Y + 230));
	OutChildNode2 = CreateSkillTreeNode(ChildNode2SkillID, FVector2D(Parent->GetNodePosition().X + 90, Parent->GetNodePosition().Y + 230));
}

void USkillTree::CreateThreeChildNode(USkillTreeNode* Parent, FName ChildNode1SkillID, FName ChildNode2SkillID, FName ChildNode3SkillID, USkillTreeNode*& OutChildNode1,
	USkillTreeNode*& OutChildNode2, USkillTreeNode*& OutChildNode3)
{
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X, Parent->GetNodePosition().Y + 80));

	if (USkillTreeNodeLine* NodeLine = CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X - 10, Parent->GetNodePosition().Y + 160)))
	{
		NodeLine->SetLineLength(200);
		NodeLine->SetRenderTransformPivot(FVector2D(1,0));
		NodeLine->SetRenderTransformAngle(90);
	}
	if (USkillTreeNodeLine* NodeLine = CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X + 10, Parent->GetNodePosition().Y + 160)))
	{
		NodeLine->SetLineLength(200);
		NodeLine->SetRenderTransformPivot(FVector2D(0,0));
		NodeLine->SetRenderTransformAngle(-90);
	}
	
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X, Parent->GetNodePosition().Y + 150));
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X - 200, Parent->GetNodePosition().Y + 150));
	CreateSkillTreeNodeLine(FVector2D(Parent->GetNodePosition().X + 200, Parent->GetNodePosition().Y + 150));

	OutChildNode1 = CreateSkillTreeNode(ChildNode1SkillID, FVector2D(Parent->GetNodePosition().X - 200, Parent->GetNodePosition().Y + 230));
	OutChildNode2 = CreateSkillTreeNode(ChildNode2SkillID, FVector2D(Parent->GetNodePosition().X, Parent->GetNodePosition().Y + 230));
	OutChildNode3 = CreateSkillTreeNode(ChildNode3SkillID, FVector2D(Parent->GetNodePosition().X + 200, Parent->GetNodePosition().Y + 230));
}
