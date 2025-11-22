// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/ANFS_InputBuffer.h"

#include "GAS/SoulAbilitySystemComponent.h"
#include "PSoul/SoulGameplayTags.h"

void UANFS_InputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(USoulAbilitySystemComponent* ASC = MeshComp->GetOwner()->FindComponentByClass<USoulAbilitySystemComponent>())
	{
		ASC->AddLooseGameplayTag(InputBufferTag);
	}
}

void UANFS_InputBuffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(USoulAbilitySystemComponent* ASC = MeshComp->GetOwner()->FindComponentByClass<USoulAbilitySystemComponent>())
	{
		ASC->ConsumeInputBuffer();
		ASC->RemoveLooseGameplayTag(InputBufferTag);
	}
	
}

