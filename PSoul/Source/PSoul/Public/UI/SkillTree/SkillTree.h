// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "SkillTree.generated.h"

class USkillTreeManager;
class USkillTreeNodeLine;
class USkillTreeNodeData;
class UCanvasPanel;
class USkillTreeData;
class USkillTreeNode;
/**
 * 
 */
UCLASS()
class PSOUL_API USkillTree : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CP_SkillTree;

	UPROPERTY(EditDefaultsOnly)
	USkillTreeData* PrewSkillTreeData;

	USkillTreeData* SkillTreeData;
	
	TObjectPtr<USkillTreeManager> SkillTreeManagerComp = nullptr;
public:
	void InitSkillTree(USkillTreeData* InSkillTreeData);
private:
	void BuildSkillTree();
	void UpdateSkillTree();
	USkillTreeNode* CreateSkillTreeNode(FName InSkillID, FVector2D InNodePosition = FVector2D::Zero());
	USkillTreeNodeLine* CreateSkillTreeNodeLine(FVector2D InNodePosition = FVector2D::Zero());
	void CreateChildNodes(USkillTreeNode* Parent);
	void CreateOneChildNode(USkillTreeNode* Parent, FName ChildNodeSkillID, USkillTreeNode*& OutChildNode);
	void CreateTwoChildNode(USkillTreeNode* Parent, FName ChildNode1SkillID, FName ChildNode2SkillID, USkillTreeNode*& OutChildNode1, USkillTreeNode*& OutChildNode2);
	void CreateThreeChildNode(USkillTreeNode* Parent, FName ChildNode1SkillID, FName ChildNode2SkillID, FName ChildNode3SkillID, USkillTreeNode*& OutChildNode1, USkillTreeNode*& OutChildNode2, USkillTreeNode*& OutChildNode3);
	
	TArray<UUserWidget*> AllWidgets;
};
