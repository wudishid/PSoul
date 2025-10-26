// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeManager.generated.h"

class USkillTreeNodeData;
class USkillTreeData;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillUnlocked, TArray<FName>);

UCLASS()
class PSOUL_API USkillTreeManager : public UActorComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	TArray<USkillTreeData*> GetSkillTreeDatas() const { return SkillTreeDatas; }
	
	bool TryLearnSkill(FName InSkillID);

	USkillTreeNodeData* GetSkillTreeNodeData(FName InSkillID);

	bool IsSkillUnlocked(FName InSkillID);
	
	FOnSkillUnlocked OnSkillUnlocked;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<USkillTreeData*> SkillTreeDatas;

	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<FName> UnlockedSkills;
};
