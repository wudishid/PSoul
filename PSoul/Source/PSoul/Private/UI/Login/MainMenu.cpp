// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Login/MainMenu.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SoulGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	Overlay_Loading->SetVisibility(ESlateVisibility::Hidden);
	Btn_EnterGame->OnClicked.AddDynamic(this, &ThisClass::HandleEnterGameClicked);
}

void UMainMenu::HandleEnterGameClicked()
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
			UGameplayStatics::OpenLevel(GetWorld(), "47.106.12.36");
			//UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MapToLoad);
		}
	}));
	
}
