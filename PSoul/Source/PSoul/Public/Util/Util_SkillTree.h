#pragma once
#include "CoreMinimal.h"

class USkillTreeData;
class USkillTreeNodeData;

namespace Util_SkillTree
{
	TArray<USkillTreeNodeData*> GetSkillUnlockedSkillNodeDatas(USkillTreeData* SkillTreeData, FName SkillID);
}
