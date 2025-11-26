// Fill out your copyright notice in the Description page of Project Settings.
#include "Anim/AnimNotify/ANFS_DamageCheck.h"
#include "Components/DamageCheckComponent.h"


void UANFS_DamageCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (!OwnerActor->HasAuthority()) return;
		
		if (UDamageCheckComponent* DamageCheckComponent = OwnerActor->FindComponentByClass<UDamageCheckComponent>())
		{
			DamageCheckComponent->StartCheck(DamageInfo);
		}
	}
}

void UANFS_DamageCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (!OwnerActor->HasAuthority()) return;
		
		if (UDamageCheckComponent* DamageCheckComponent = OwnerActor->FindComponentByClass<UDamageCheckComponent>())
		{
			DamageCheckComponent->CheckDamage(bForceUseBoxTrace);
		}
	}
}

void UANFS_DamageCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (!OwnerActor->HasAuthority()) return;
		
		if (UDamageCheckComponent* DamageCheckComponent = OwnerActor->FindComponentByClass<UDamageCheckComponent>())
		{
			DamageCheckComponent->EndCheck();
		}
	}
}


