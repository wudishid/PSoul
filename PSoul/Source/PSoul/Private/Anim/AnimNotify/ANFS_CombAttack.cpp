// Fill out your copyright notice in the Description page of Project Settings.
#include "Anim/AnimNotify/ANFS_CombAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "PSoul/SoulGameplayTags.h"


void UANFS_CombAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	FGameplayEventData EventData;
	EventData.EventTag = SoulGameplayTags::GameplayEvent_Montage_OpenCombWindow;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventData.EventTag, EventData);
}

void UANFS_CombAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	FGameplayEventData EventData;
	EventData.EventTag = SoulGameplayTags::GameplayEvent_Montage_CloseCombWindow;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventData.EventTag, EventData);
	
}
