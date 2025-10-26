// Fill out your copyright notice in the Description page of Project Settings.
#include "SkillTreeSystem/SkillTreeManager.h"
#include "SkillTreeSystem/SkillTreeData.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"
#include "Util/Util_SkillTree.h"

void USkillTreeManager::BeginPlay()
{
	Super::BeginPlay();
}

bool USkillTreeManager::TryLearnSkill(FName InSkillID)
{
	for (auto& SkillTreeData : SkillTreeDatas)
	{
		TArray<USkillTreeNodeData*> UnlockedSkillNodeDatas = Util_SkillTree::GetSkillUnlockedSkillNodeDatas(SkillTreeData, InSkillID);
		if (UnlockedSkillNodeDatas.Num() > 0)
		{
			for (auto& UnlockedSkillNodeData : UnlockedSkillNodeDatas)
			{
				UnlockedSkills.AddUnique(UnlockedSkillNodeData->SkillID);
			}

			OnSkillUnlocked.Broadcast(UnlockedSkills);
		}
			
		return true;
	}
	
	return false;
}

USkillTreeNodeData* USkillTreeManager::GetSkillTreeNodeData(FName InSkillID)
{
	if (SkillTreeDatas.Num() < 0) return nullptr;
	
	for (auto& SkillTreeData : SkillTreeDatas)
	{
		if (SkillTreeData->RootNodeData->SkillID == InSkillID)
		{
			return SkillTreeData->RootNodeData;
		}

		for (auto& pair : SkillTreeData->SkillNodeDatas)
		{
			for (auto& nodeData : pair.Value.SkillTreeNodes)
			{
				if (nodeData->SkillID == InSkillID)
				{
					return nodeData;
				}
			}
		}
	}
	
	return nullptr;
}

bool USkillTreeManager::IsSkillUnlocked(FName InSkillID)
{
	return UnlockedSkills.Contains(InSkillID);
}
