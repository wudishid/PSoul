// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/LoginPanel.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SoulGameInstance.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void ULoginPanel::NativeConstruct()
{
	Super::NativeConstruct();

	RedTeamCheckBox->SetIsChecked(true);
	BlueTeamCheckBox->SetIsChecked(false);
	Overlay_Loading->SetVisibility(ESlateVisibility::Hidden);
	
	RedTeamCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::HandleRedTeamCheckBoxChecked);
	BlueTeamCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::HandleBlueTeamCheckBoxChecked);
	Btn_EnterGame->OnClicked.AddDynamic(this, &ThisClass::HandleEnterGameClicked);
}

void ULoginPanel::HandleRedTeamCheckBoxChecked(bool bIsChecked)
{
	if(bIsChecked)
	{
		BlueTeamCheckBox->SetIsChecked(false);
		if(USoulGameInstance* GameInstance = GetGameInstance<USoulGameInstance>())
		{
			GameInstance->Team = ESoulCharacterTeam::RedPlayer;
		}
	}
}

void ULoginPanel::HandleBlueTeamCheckBoxChecked(bool bIsChecked)
{
	if(bIsChecked)
	{
		RedTeamCheckBox->SetIsChecked(false);
		if(USoulGameInstance* GameInstance = GetGameInstance<USoulGameInstance>())
		{
			GameInstance->Team = ESoulCharacterTeam::BluePlayer;
		}
	}
}

void ULoginPanel::HandleEnterGameClicked()
{
	Overlay_Loading->SetVisibility(ESlateVisibility::Visible);
	Btn_EnterGame->SetVisibility(ESlateVisibility::Hidden);
	
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
		
		if(USoulGameInstance* GameInstance = GetGameInstance<USoulGameInstance>())
		{
			//UGameplayStatics::OpenLevel(GetWorld(), "47.106.12.36");
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MapToLoad);
		}
	}));
	
}
