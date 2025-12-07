// Fill out your copyright notice in the Description page of Project Settings.
#include "PSoul/Public/UI/Lobby/MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SoulGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	Overlay_Loading->SetVisibility(ESlateVisibility::Hidden);
	VB_Option->SetVisibility(ESlateVisibility::Hidden);
	
	Btn_SinglePlayerGame->OnClicked.AddDynamic(this, &ThisClass::HandleSinglePlayerGameClicked);
	Btn_MultiPlayerGame->OnClicked.AddDynamic(this, &ThisClass::HandleMultiPlayerGameClicked);
	Btn_ExitGame->OnClicked.AddDynamic(this, &ThisClass::HandleExitGameClicked);
	
	TextBox_Name->OnTextCommitted.AddDynamic(this, &ThisClass::HandleNameTextBoxCommited);
}

void UMainMenu::HandleSinglePlayerGameClicked()
{
	Overlay_Loading->SetVisibility(ESlateVisibility::Visible);
	VB_Option->SetVisibility(ESlateVisibility::Hidden);
	
	TSharedPtr<FStreamableHandle> Handle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(MapToLoad.ToSoftObjectPath());
	Handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateLambda([this](TSharedRef<FStreamableHandle> Handle)
	{
		ProgressBar_Load->SetPercent(Handle->GetProgress());
		TextBlock_LoadPercentage->SetText(UKismetTextLibrary::AsPercent_Float(Handle->GetProgress(), ToPositiveInfinity));
	}));

	Handle->BindCompleteDelegate(FStreamableDelegate::CreateLambda([this]()
	{
		float Percent = 1.f;
		ProgressBar_Load->SetPercent(Percent);
		TextBlock_LoadPercentage->SetText(UKismetTextLibrary::AsPercent_Float(Percent, ToPositiveInfinity));

		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MapToLoad);
	}));
}

void UMainMenu::HandleMultiPlayerGameClicked()
{
	Overlay_Loading->SetVisibility(ESlateVisibility::Visible);
	VB_Option->SetVisibility(ESlateVisibility::Hidden);
	
	TSharedPtr<FStreamableHandle> Handle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(MapToLoad.ToSoftObjectPath());
	Handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateLambda([this](TSharedRef<FStreamableHandle> Handle)
	{
		ProgressBar_Load->SetPercent(Handle->GetProgress());
		TextBlock_LoadPercentage->SetText(UKismetTextLibrary::AsPercent_Float(Handle->GetProgress(), ToPositiveInfinity));
	}));

	Handle->BindCompleteDelegate(FStreamableDelegate::CreateLambda([this]()
	{
		float Percent = 1.f;
		ProgressBar_Load->SetPercent(Percent);
		TextBlock_LoadPercentage->SetText(UKismetTextLibrary::AsPercent_Float(Percent, ToPositiveInfinity));

		GetOwningPlayer()->ClientTravel(TEXT("47.106.12.36"), TRAVEL_Absolute);
	}));
}

void UMainMenu::HandleExitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenu::HandleNameTextBoxCommited(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (!InText.IsEmpty())
	{
		if (USoulGameInstance* SoulGameInstance = GetGameInstance<USoulGameInstance>())
		{
			SoulGameInstance->PlayerName = *InText.ToString();
			VB_Option->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		VB_Option->SetVisibility(ESlateVisibility::Hidden);
	}
}


