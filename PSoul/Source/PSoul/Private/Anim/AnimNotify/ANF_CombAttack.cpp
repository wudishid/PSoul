// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim/AnimNotify/ANF_CombAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "PSoul/SoulGameplayTags.h"


void UANF_CombAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	FGameplayEventData EventData;
	EventData.EventTag = SoulGameplayTags::GameplayEvent_Montage_CombWindow;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventData.EventTag, EventData);
	//UKismetSystemLibrary::PrintString(GetWorld(), "NextComb", true, true, FLinearColor::Yellow, 12.f);
}
