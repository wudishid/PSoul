// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "ItemInfoWidget.generated.h"

struct FInventoryItemInfo;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PSOUL_API UItemInfoWidget : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void UpdateItemInfo(const FInventoryItemInfo& InItemInfo);
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemDesc;
};
