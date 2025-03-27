// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "InventorySlot.generated.h"

struct FInventoryItemSlot;
class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UInventorySlot : public USoulUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSlot(FInventoryItemSlot& ItemSlot);
	bool IsEmpty() const;
	int32 SlotIndex;
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Amount;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Slot;

	bool bEmpty;
	
};
