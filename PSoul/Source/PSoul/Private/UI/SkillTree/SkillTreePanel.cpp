// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/SkillTree/SkillTreePanel.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Development/Soul_UISetting.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "Kismet/KismetTextLibrary.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "UI/SkillTree/SkillTree.h"


void USkillTreePanel::Init()
{
	HB_SkillTreePanel->ClearChildren();
	
	SkillTreeManagerComp = GetOwningPlayerPawn()->FindComponentByClass<USkillTreeManager>();
	checkf(SkillTreeManagerComp, TEXT("SkillTreeManagerComp Is not valid!"));
	SkillTreeManagerComp->UnlockSkillTreeRootSkill();
	
	TextBlock_SkillPoint->SetText(UKismetTextLibrary::Conv_IntToText(SkillTreeManagerComp->GetAvailableSkillPoint()));
	SkillTreeManagerComp->OnSkillLearned.AddUObject(this, &ThisClass::OnSkillLearned);
	if (USoulAbilitySystemComponent* ASC = GetOwningPlayerPawn()->FindComponentByClass<USoulAbilitySystemComponent>())
	{
		if (!ASC->GetGameplayAttributeValueChangeDelegate(ASC->GetSet<USoulPlayerSet>()->GetLevelAttribute()).IsBoundToObject(this))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(ASC->GetSet<USoulPlayerSet>()->GetLevelAttribute()).AddUObject(this, &ThisClass::HandleLevelChanged);
		}
	}
	
	for (auto &SkillTreeData : SkillTreeManagerComp->GetSkillTreeDatas())
	{
		if (USkillTree* SkillTreeWidget = CreateWidget<USkillTree>(GetOwningPlayer(), GetDefault<USoul_UISetting>()->SkillTreeClass.LoadSynchronous()))
		{
			SkillTreeWidget->InitSkillTree(SkillTreeData);
			HB_SkillTreePanel->AddChildToHorizontalBox(SkillTreeWidget);
		}
	}
}

void USkillTreePanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreePanel::HandleLevelChanged(const FOnAttributeChangeData& ChangeData)
{
	TextBlock_SkillPoint->SetText(UKismetTextLibrary::Conv_IntToText(SkillTreeManagerComp->GetAvailableSkillPoint()));
}

void USkillTreePanel::OnSkillLearned(TArray<FName> InLearnedSkillsID)
{
	TextBlock_SkillPoint->SetText(UKismetTextLibrary::Conv_IntToText(SkillTreeManagerComp->GetAvailableSkillPoint()));
}


