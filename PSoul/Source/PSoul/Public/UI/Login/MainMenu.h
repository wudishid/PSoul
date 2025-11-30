// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "MainMenu.generated.h"

class UOverlay;
class UTextBlock;
class UProgressBar;
class UButton;
class UCheckBox;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UMainMenu : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void HandleEnterGameClicked();
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_EnterGame;

	UPROPERTY(meta = (BindWidget))
	UOverlay* Overlay_Loading;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Load;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_LoadPercentage;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> MapToLoad;
};
