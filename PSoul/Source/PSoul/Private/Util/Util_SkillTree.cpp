// Fill out your copyright notice in the Description page of Project Settings.

#include "Util/Util_SkillTree.h"
#include "SkillTreeSystem/SkillTreeData.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"


TArray<USkillTreeNodeData*> Util_SkillTree::GetSkillUnlockedSkillNodeDatas(USkillTreeData* SkillTreeData, FName SkillID)
{
	for (auto& pair : SkillTreeData->SkillNodeDatas)
	{
		if (pair.Key == SkillID)
		{
			return pair.Value.SkillTreeNodes;
		}
	}
	
	return TArray<USkillTreeNodeData*>();
}

