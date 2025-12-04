// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Player/Attribute/AttributePanel.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/SoulCharacterBase.h"
#include "UI/Player/Attribute/AttributePoint.h"
#include "UI/Player/Attribute/AttributeRow.h"

void UAttributePanel::Init()
{
	if (ASoulCharacterBase* CharacterBase = GetOwningPlayerPawn<ASoulCharacterBase>())
	{
		TextBlock_NickName->SetText(FText::FromString(CharacterBase->GetCharacterName().ToString()));
	}

	AttributeRow_Level->Init();
	AttributeRow_NextLevelNeedSoul->Init();
	AttributePoint_Life->Init();
	AttributePoint_Strength->Init();
	AttributePoint_Stamina->Init();
	AttributeRow_Health->Init();
	AttributeRow_Stamina->Init();
	AttributeRow_Strength->Init();
	AttributeRow_PhysicalDefence->Init();
	AttributeRow_MagicDefence->Init();
	AttributeRow_MaxSpeed->Init();
	AttributeRow_Soul->Init();
	
}
