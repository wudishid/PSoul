// Fill out your copyright notice in the Description page of Project Settings.
#include "SkillTreeSystem/SkillTreeData.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"


FSkillTreeNodes* USkillTreeData::GetSkillTreeNodesBySkillID(FName InSkillID)
{
	for (auto& SkillNodeData : SkillNodeDatas)
	{
		if (SkillNodeData.Key->SkillID == InSkillID)
		{
			return &SkillNodeData.Value;
		}
	}
	return nullptr;
}
