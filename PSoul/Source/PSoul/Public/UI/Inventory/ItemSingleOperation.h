// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "ItemSingleOperation.generated.h"

class UTextBlock;
enum class EItemOpetaionType : uint8;
class UButton;
class UInventorySlot;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnOperationClicked, EItemOpetaionType)


UCLASS(Abstract)
class PSOUL_API UItemSingleOperation : public USoulUserWidget
{
	GENERATED_BODY()
public:
	void UpdateSingleOperation(EItemOpetaionType OperationType);
	FOnOperationClicked OnOperationClicked;
protected:
	virtual void NativeConstruct() override;
	
	EItemOpetaionType ItemOpetaionType;
	
	UFUNCTION()
	void HandleBtn_OperateClicked();
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Operate;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_OperationName;
};
