// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeManager.generated.h"

class USkillTreeData;
/**
 * 
 */
UCLASS()
class PSOUL_API USkillTreeManager : public UActorComponent
{
	GENERATED_BODY()
public:
	
	TArray<USkillTreeData*> GetSkillDatas() const
	{
		return SkillDatas;
	}
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeManager")
	TArray<USkillTreeData*> SkillDatas;
};
