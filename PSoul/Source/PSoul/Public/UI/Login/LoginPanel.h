// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "LoginPanel.generated.h"

class UButton;
class UCheckBox;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API ULoginPanel : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleRedTeamCheckBoxChecked(bool bIsChecked);

	UFUNCTION()
	void HandleBlueTeamCheckBoxChecked(bool bIsChecked);
	
	UFUNCTION()
	void HandleEnterGameClicked();
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* RedTeamCheckBox;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* BlueTeamCheckBox;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_EnterGame;
	
};
