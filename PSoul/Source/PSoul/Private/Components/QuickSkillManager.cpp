// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/QuickSkillManager.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "PSoul/SoulGameplayTags.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "SkillTreeSystem/SkillTreeNodeData.h"
#include "SkillTreeSystem/SoulSkillTreeAbility.h"


// Sets default values for this component's properties
UQuickSkillManager::UQuickSkillManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	QuickSkillSlots.Add(SoulGameplayTags::InputTag_QuickSkill1, NAME_None);
	QuickSkillSlots.Add(SoulGameplayTags::InputTag_QuickSkill2, NAME_None);
	QuickSkillSlots.Add(SoulGameplayTags::InputTag_QuickSkill3, NAME_None);
	QuickSkillSlots.Add(SoulGameplayTags::InputTag_QuickSkill4, NAME_None);
}

// Called when the game starts
void UQuickSkillManager::BeginPlay()
{
	Super::BeginPlay();
}

void UQuickSkillManager::OnSetPawn(APawn* InPawn)
{
	if (InPawn)
	{
		SkillTreemanager = InPawn->FindComponentByClass<USkillTreeManager>();
		check(SkillTreemanager);
		ASC = InPawn->FindComponentByClass<USoulAbilitySystemComponent>();
		check(ASC);
	}
}

void UQuickSkillManager::PressSkill(FGameplayTag InSkillInputTag)
{
	if (!SkillTreemanager) return;
	if (!ASC) return;
	
	if (QuickSkillSlots.Contains(InSkillInputTag))
	{
		FName SkillID = *QuickSkillSlots.Find(InSkillInputTag);
		if (!SkillID.IsNone())
		{
			if (USkillTreeNodeData* SkillTreeNodeData = SkillTreemanager->GetSkillTreeNodeData(SkillID))
			{
				ASC->TryActivateAbilityByClass(SkillTreeNodeData->SkillClass);
			}
		}
	}
}

void UQuickSkillManager::SetQuickSkill(FGameplayTag InSkillInputTag, FName InSkillID)
{
	if (QuickSkillSlots.Contains(InSkillInputTag))
	{
		*QuickSkillSlots.Find(InSkillInputTag) = InSkillID;
		OnQuickSkillChanged.Broadcast(InSkillInputTag);
	}
}


