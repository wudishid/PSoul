// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Soul_UISetting.generated.h"

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
	
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
