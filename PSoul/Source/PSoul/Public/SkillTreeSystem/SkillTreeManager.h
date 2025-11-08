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
	TArray<USkillTreeData*> GetSkillTreeDatas() const { return SkillTreeDatas; }
	
	bool TryLearnSkill(FName InSkillID);
	
	USkillTreeNodeData* GetSkillTreeNodeData(FName InSkillID) const;

	bool IsSkillUnlocked(FName InSkillID) const;
	bool IsSkillLearned(FName InSkillID) const;

	//技能是否可以释放
	bool CanSkillRelease(FName InSkillID) const;
	
	int32 GetAvailableSkillPoint() const;
	
	FOnSkillUnlocked OnSkillUnlocked;
	FOnSkillLearned OnSkillLearned;
private:
	int32 GetCostedSkillPoints() const;
	void LearnSkill(FName InSkillID);
	void UnlockSkillTreeRootSkill();
	
	UFUNCTION(Server, Reliable)
	void ServerGiveSkill(FName InSkillID);

	UFUNCTION()
	void OnRep_LearnedSkills();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<USkillTreeData*> SkillTreeDatas;

	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<FName> UnlockedSkills;

	UPROPERTY(ReplicatedUsing = OnRep_LearnedSkills, EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<FName> LearnedSkills;

	UPROPERTY()
	TObjectPtr<USoulAbilitySystemComponent> ASC;
};


