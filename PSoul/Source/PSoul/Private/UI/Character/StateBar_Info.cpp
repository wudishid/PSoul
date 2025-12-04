// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Character/StateBar_Info.h"
#include "Components/TextBlock.h"
#include "GameFramework/SoulCharacterBase.h"
#include "UI/Character/StateBar.h"

void UStateBar_Info::Init(APawn* OwnerPawn, FGameplayAttribute InAttribute, FGameplayAttribute InMaxAttribute)
{
	if (ASoulCharacterBase* SoulCharacterBase = Cast<ASoulCharacterBase>(OwnerPawn))
	{
		Text_Name->SetText(FText::FromName(SoulCharacterBase->GetCharacterName()));
	}
	StateBar->Init(OwnerPawn, InAttribute, InMaxAttribute);
}
