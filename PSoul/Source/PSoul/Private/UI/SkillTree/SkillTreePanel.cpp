// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SkillTree/SkillTreePanel.h"
#include "Components/HorizontalBox.h"
#include "Development/Soul_UISetting.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "UI/SkillTree/SkillTree.h"

void USkillTreePanel::NativeConstruct()
{
	Super::NativeConstruct();

	HB_SkillTreePanel->ClearChildren();
	
	SkillTreeManagerComp = GetOwningPlayerPawn()->FindComponentByClass<USkillTreeManager>();
	checkf(SkillTreeManagerComp, TEXT("SkillTreeManagerComp Is not valid!"));
	
	for (auto &SkillTreeData : SkillTreeManagerComp->GetSkillTreeDatas())
	{
		if (USkillTree* SkillTreeWidget = CreateWidget<USkillTree>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->SkillTreeClass.LoadSynchronous()))
		{
			SkillTreeWidget->InitSkillTree(SkillTreeData);
			HB_SkillTreePanel->AddChildToHorizontalBox(SkillTreeWidget);
		}
	}
	
}
