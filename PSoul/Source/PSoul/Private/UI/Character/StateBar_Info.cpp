// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Character/StateBar_Info.h"
#include "Components/TextBlock.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "UI/Character/StateBar.h"

void UStateBar_Info::OnPlayerNameChanged(FString NewName)
{
	Text_Name->SetText(FText::FromString(NewName));
}

void UStateBar_Info::Init(APawn* OwnerPawn, FGameplayAttribute InAttribute, FGameplayAttribute InMaxAttribute)
{
	if (ASoulCharacterBase* SoulCharacterBase = Cast<ASoulCharacterBase>(OwnerPawn))
	{
		Text_Name->SetText(FText::FromName(SoulCharacterBase->GetCharacterName()));
		if (ASoulPlayerState_Game* PlayerState_Game = SoulCharacterBase->GetPlayerState<ASoulPlayerState_Game>())
		{
			PlayerState_Game->OnPlayerNameChanged.RemoveAll(this);
			PlayerState_Game->OnPlayerNameChanged.AddUObject(this, &ThisClass::OnPlayerNameChanged);
		}
	}
	
	StateBar->Init(OwnerPawn, InAttribute, InMaxAttribute);
}
