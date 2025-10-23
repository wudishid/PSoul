// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SkillTree/SkillTreeNodeLine.h"

#include "Components/SizeBox.h"

void USkillTreeNodeLine::SetLineLength(float InLength)
{
	SB_Line->SetHeightOverride(InLength);
}
