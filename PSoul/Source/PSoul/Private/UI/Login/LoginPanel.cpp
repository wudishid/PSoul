// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/LoginPanel.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "GameFramework/SoulGameInstance.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

void ULoginPanel::NativeConstruct()
{
	Super::NativeConstruct();

	RedTeamCheckBox->SetIsChecked(true);
	BlueTeamCheckBox->SetIsChecked(false);
	
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
	if(USoulGameInstance* GameInstance = GetGameInstance<USoulGameInstance>())
	{
		UGameplayStatics::OpenLevel(GetWorld(), "47.106.12.36");
	}
}
