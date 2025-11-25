// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeManager.generated.h"

class USoulAbilitySystemComponent;
class USkillTreeNodeData;
class USkillTreeData;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillUnlocked, TArray<FName>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillLearned, TArray<FName>);

UCLASS()
class PSOUL_API USkillTreeManager : public UActorComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	void UnlockSkillTreeRootSkill();
	
	TArray<USkillTreeData*> GetSkillTreeDatas() const { return SkillTreeDatas; }
	
	bool TryLearnSkill(FName InSkillID);
	
	void UnlockSkill(FName InSkillID);
	
	USkillTreeNodeData* GetSkillTreeNodeData(FName InSkillID) const;

	bool IsSkillUnlocked(FName InSkillID) const;
	bool IsSkillLearned(FName InSkillID) const;

	//技能是否可以释放
	bool CanSkillRelease(FName InSkillID) const;
	
	int32 GetAvailableSkillPoint() const;
	
	FOnSkillUnlocked OnSkillUnlocked;
	FOnSkillLearned OnSkillLearned;

	const TArray<FName>& GetUnlockedSkills() const { return UnlockedSkills; }
	const TArray<FName>& GetLearnedSkills() const { return LearnedSkills; }
private:
	int32 GetCostedSkillPoints() const;

	UFUNCTION(Server, Reliable)
	void LearnSkill(FName InSkillID);
	
	UFUNCTION()
	void OnRep_LearnedSkills();

	UFUNCTION()
	void OnRep_UnlockedSkills();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<USkillTreeData*> SkillTreeDatas;

	UPROPERTY(ReplicatedUsing = OnRep_UnlockedSkills, EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<FName> UnlockedSkills;

	UPROPERTY(ReplicatedUsing = OnRep_LearnedSkills, EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<FName> LearnedSkills;
};


