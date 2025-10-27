// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillTree/SkillInfoTip.h"

#include "Components/TextBlock.h"

void USkillInfoTip::InitTip(FText InTip)
{
	TextBlock_Tip->SetText(InTip);
}
