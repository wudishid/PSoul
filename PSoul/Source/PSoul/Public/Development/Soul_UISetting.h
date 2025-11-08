// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Soul_UISetting.generated.h"

class USkillPicklQuickSlot;
class USkillQuickSlot;
class USkillQuickPickPanel;
class USkillQuickPanel;
class USkillInfoTip;
class USkillTree;
class USkillTreeNode;
class USkillTreeNodeLine;
class UItemInfoWidget;
class UItemSingleOperation;
class UItemOperationPanel;
class UInventorySlot;
/**
 * 
 */
UCLASS(config = Game, DefaultConfig)
class PSOUL_API USoul_UISetting : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | Inventory")
	TSoftClassPtr<UInventorySlot> InventorySlotClass;
	
	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | Inventory")
	TSoftClassPtr<UItemOperationPanel> ItemOperationPanelClass;

	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | Inventory")
	TSoftClassPtr<UItemSingleOperation> ItemSingleOperationClass;

	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | Inventory")
	TSoftClassPtr<UItemInfoWidget> ItemInfoClass;


	
	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | SkillTree")
	TSoftClassPtr<USkillTree> SkillTreeClass;
	
	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | SkillTree")
	TSoftClassPtr<USkillTreeNode> SkillTreeNodeClass;
	
	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | SkillTree")
	TSoftClassPtr<USkillTreeNodeLine> SkillTreeNodeLineClass;

	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | SkillTree")
	TSoftClassPtr<USkillInfoTip> SkillInfoTipClass;


	
	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | QuickSkill")
	TSoftClassPtr<USkillQuickPanel> SkillQuickPanelClass;

	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | QuickSkill")
	TSoftClassPtr<USkillQuickPickPanel> SkillQuickPickPanelClass;

	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | QuickSkill")
	TSoftClassPtr<USkillQuickSlot> SkillQuickSlotClass;

	UPROPERTY(EditAnywhere, Config, Category = "Soul UI Settings | QuickSkill")
	TSoftClassPtr<USkillPicklQuickSlot> SkillPicklQuickSlotClass;
	
protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
