// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/SkillTreeData.h"

#include "SkillTreeSystem/SkillTreeNodeData.h"

USkillTreeNodeData* USkillTreeData::GetSkillTreeNodeData(FName SkillID)
{
	if (RootNodeData->SkillID == SkillID)
	{
		return RootNodeData;
	}

	for (auto& pair : SkillNodeDatas)
	{
		for (auto& nodeData : pair.Value.SkillTreeNodes)
		{
			if (SkillID == nodeData->SkillID)
			{
				return nodeData;
			}
		}
	}
	
	return nullptr;
}
