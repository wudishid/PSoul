// Fill out your copyright notice in the Description page of Project Settings.
#include "SkillTreeSystem/SkillTreeManager.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "Net/UnrealNetwork.h"
#include "SkillTreeSystem/SkillTreeData.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"
#include "SkillTreeSystem/SoulSkillTreeAbility.h"

void USkillTreeManager::BeginPlay()
{
	Super::BeginPlay();
	
	ASC = GetOwner()->FindComponentByClass<USoulAbilitySystemComponent>();
	check(ASC);

	UnlockSkillTreeRootSkill();
}

void USkillTreeManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(USkillTreeManager, LearnedSkills, COND_None);
}

bool USkillTreeManager::TryLearnSkill(FName InSkillID)
{
	if (USkillTreeNodeData* LearnedSkillNode = GetSkillTreeNodeData(InSkillID))
	{
		if (GetAvailableSkillPoint() >= LearnedSkillNode->SkillPointCost)
		{
			LearnSkill(InSkillID);
			return true;
		}
	}
	
	return false;
}

USkillTreeNodeData* USkillTreeManager::GetSkillTreeNodeData(FName InSkillID) const
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

bool USkillTreeManager::IsSkillLearned(FName InSkillID) const
{
	return LearnedSkills.Contains(InSkillID);
}

bool USkillTreeManager::CanSkillRelease(FName InSkillID) const
{
	if (USkillTreeNodeData* SkillTreeNodeData = GetSkillTreeNodeData(InSkillID))
	{
		return SkillTreeNodeData->SkillType == ESkillType::Active;
	}
	
	return false;
}

int32 USkillTreeManager::GetAvailableSkillPoint() const
{
	int32 HoldSkillPoints = ASC->GetSet<USoulPlayerSet>()->GetLevel() - 1;
	int32 AvailableSkillPoint = HoldSkillPoints - GetCostedSkillPoints();
	return AvailableSkillPoint;
}

int32 USkillTreeManager::GetCostedSkillPoints() const
{
	if (LearnedSkills.IsEmpty()) return 0;

	int32 ResultPoints = 0;
	for (auto& LearnedSkill : LearnedSkills)
	{
		if (USkillTreeNodeData* SkillTreeNode = GetSkillTreeNodeData(LearnedSkill))
		{
			ResultPoints += SkillTreeNode->SkillPointCost;
		}
	}
	return ResultPoints;
}

void USkillTreeManager::LearnSkill(FName InSkillID)
{
	ServerGiveSkill(InSkillID);
	
	//解锁该技能的后置技能
	for (auto& SkillTreeData : SkillTreeDatas)
	{
		if (FSkillTreeNodes* SkillTreeNodes = SkillTreeData->GetSkillTreeNodesBySkillID(InSkillID))
		{
			if (!SkillTreeNodes->SkillTreeNodes.IsEmpty())
			{
				for (auto& UnlockedSkillNodeData : SkillTreeNodes->SkillTreeNodes)
				{
					UnlockedSkills.AddUnique(UnlockedSkillNodeData->SkillID);
				}
			
				OnSkillUnlocked.Broadcast(UnlockedSkills);
			}
		}
	}
}

void USkillTreeManager::UnlockSkillTreeRootSkill()
{
	for (auto& SkillTreeData : SkillTreeDatas)
	{
		UnlockedSkills.AddUnique(SkillTreeData->RootNodeData->SkillID);
	}
}

void USkillTreeManager::ServerGiveSkill_Implementation(FName InSkillID)
{
	if (USkillTreeNodeData* LearnedSkillNode = GetSkillTreeNodeData(InSkillID))
	{
		FGameplayAbilitySpec AbilitySpec(LearnedSkillNode->SkillClass);
		ASC->GiveAbility(AbilitySpec);
		LearnedSkills.AddUnique(InSkillID);
	}
}

void USkillTreeManager::OnRep_LearnedSkills()
{
	OnSkillLearned.Broadcast(LearnedSkills);
}

bool USkillTreeManager::IsSkillUnlocked(FName InSkillID) const
{
	return UnlockedSkills.Contains(InSkillID);
}
