// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "MainMenu.generated.h"

class UVerticalBox;
class UEditableTextBox;
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
	void HandleSinglePlayerGameClicked();

	UFUNCTION()
	void HandleMultiPlayerGameClicked();

	UFUNCTION()
	void HandleExitGameClicked();

	UFUNCTION()
	void HandleNameTextBoxCommited(const FText& InText, ETextCommit::Type InCommitMethod);
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox_Name;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Option;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_SinglePlayerGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_MultiPlayerGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_ExitGame;
	
	UPROPERTY(meta = (BindWidget))
	UOverlay* Overlay_Loading;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Load;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_LoadPercentage;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> MapToLoad;
};
