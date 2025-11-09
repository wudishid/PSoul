// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/QuickPanel/SkillQuickSlot.h"

#include "Components/Image.h"
#include "Components/QuickSkillManager.h"
#include "Components/TextBlock.h"
#include "PSoul/SoulGameplayTags.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"

void USkillQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Image_CD->SetVisibility(ESlateVisibility::Hidden);
	Text_CD->SetVisibility(ESlateVisibility::Hidden);
}

void USkillQuickSlot::InitSlot(FGameplayTag InSkillInputTag)
{
	SkillInputTag = InSkillInputTag;
	if (InSkillInputTag == SoulGameplayTags::InputTag_QuickSkill1)
	{
		Text_SkillKey->SetText(FText::FromString(TEXT("1")));
	}
	else if (InSkillInputTag == SoulGameplayTags::InputTag_QuickSkill2)
	{
		Text_SkillKey->SetText(FText::FromString(TEXT("2")));
	}
	else if (InSkillInputTag == SoulGameplayTags::InputTag_QuickSkill3)
	{
		Text_SkillKey->SetText(FText::FromString(TEXT("3")));
	}
	else if (InSkillInputTag == SoulGameplayTags::InputTag_QuickSkill4)
	{
		Text_SkillKey->SetText(FText::FromString(TEXT("4")));
	}
	
	QuickSkillManager = GetOwningPlayer()->FindComponentByClass<UQuickSkillManager>();
	QuickSkillManager->OnQuickSkillChanged.AddUObject(this, &ThisClass::OnQuickSkillChanged);
	QuickSkillManager->OnQuickSkillReleased.AddUObject(this, &ThisClass::OnQuickSkillReleased);
	check(QuickSkillManager);
	SkillTreeManager = GetOwningPlayerPawn()->FindComponentByClass<USkillTreeManager>();
	check(SkillTreeManager);
}

void USkillQuickSlot::OnQuickSkillChanged(FGameplayTag InSkillInputTag)
{
	if (SkillInputTag == InSkillInputTag)
	{
		FName SkillID = *QuickSkillManager->GetQuickSkillSlots().Find(SkillInputTag);
		if (!SkillID.IsNone())
		{
			if (USkillTreeNodeData* NodeData = SkillTreeManager->GetSkillTreeNodeData(SkillID))
			{
				Image_SkillIcon->SetBrushFromTexture(NodeData->SkillIcon);
			}
		}
		else
		{
			Image_SkillIcon->SetBrushFromTexture(nullptr);
		}
	}
}

void USkillQuickSlot::OnQuickSkillReleased(FGameplayTag InSkillInputTag)
{
	if (SkillInputTag == InSkillInputTag)
	{
		if (Timer_CD.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(Timer_CD);
		}
		
		Image_CD->SetVisibility(ESlateVisibility::Visible);
		Text_CD->SetVisibility(ESlateVisibility::Visible);
		SkillCDTime = QuickSkillManager->GetQuickSkillCooldownRemainTime(SkillInputTag);
		
		GetWorld()->GetTimerManager().SetTimer(Timer_CD,FTimerDelegate::CreateLambda([this]()
		{
			if (UMaterialInstanceDynamic* MID = Image_CD->GetDynamicMaterial())
			{
				float Percentage = CurrentTime / SkillCDTime;
				
				if (Percentage <= 1)
				{
					MID->SetScalarParameterValue(TEXT("Percentage"), Percentage);
					float CDRemainTime = SkillCDTime - CurrentTime;
					FNumberFormattingOptions NumberFormatOptions;
					NumberFormatOptions.MaximumFractionalDigits = CDRemainTime > 1 ? 0 : 1;
					Text_CD->SetText(FText::AsNumber(CDRemainTime, &NumberFormatOptions));
					CurrentTime += GetWorld()->GetDeltaSeconds();
				}
				else
				{
					CurrentTime = 0.f;
					GetWorld()->GetTimerManager().ClearTimer(Timer_CD);
					Image_CD->SetVisibility(ESlateVisibility::Hidden);
					Text_CD->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			
		}),GetWorld()->GetDeltaSeconds(),true);
	}
}
