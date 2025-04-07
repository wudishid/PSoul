// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "PlayerPanel.generated.h"


class UInventoryList;
class UStateBar;

UCLASS(Abstract)
class PSOUL_API UPlayerPanel : public USoulUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UStateBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UStateBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UInventoryList* InventoryList;
public:
	void InitPanel();
	void SetShowInventoryPanel(bool bShow);
	bool IsShowInventoryPanel() const;
};
