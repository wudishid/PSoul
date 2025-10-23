// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeData.generated.h"

class USkillTreeNodeData;

USTRUCT()
struct FSkillTreeNodes
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeNodes")
	TArray<USkillTreeNodeData*> SkillTreeNodes;
};


UCLASS()
class PSOUL_API USkillTreeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "SkillTree")
	FName TreeName;

	UPROPERTY(EditDefaultsOnly, Category = "SkillTree")
	USkillTreeNodeData* RootNodeData;
	
	UPROPERTY(EditDefaultsOnly, Category = "SkillTree")
	TMap<FName, FSkillTreeNodes>  SkillNodeDatas;


	USkillTreeNodeData* GetSkillTreeNodeData(FName SkillID);
	
};
