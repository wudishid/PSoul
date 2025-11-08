// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeNodeData.generated.h"

class USoulSkillTreeAbility;
/**
 * 
 */

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	GainType				UMETA(DisplayName = "增益型"), 
	ReleaseType				UMETA(DisplayName = "释放型")
};


UCLASS()
class PSOUL_API USkillTreeNodeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillTreeNodeData")
	FName SkillID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillTreeNodeData")
	FText SkillName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillTreeNodeData")
	FText SkillDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillTreeNodeData")
	UTexture2D* SkillIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillTreeNodeData")
	int32 SkillPointCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillTreeNodeData")
	ESkillType SkillType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillTreeNodeData")
	TSubclassOf<USoulSkillTreeAbility> SkillClass;
};
