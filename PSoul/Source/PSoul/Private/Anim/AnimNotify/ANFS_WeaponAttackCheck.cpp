// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim/AnimNotify/ANFS_WeaponAttackCheck.h"
#include "Components/DamageCheckComponent.h"


void UANFS_WeaponAttackCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (OwnerActor->HasAuthority())
		{
			DamageCheckComponent = OwnerActor->FindComponentByClass<UDamageCheckComponent>();
			if(DamageCheckComponent)
			{
				DamageCheckComponent->StartCheck();
			}
		}
	}
}

void UANFS_WeaponAttackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (DamageCheckComponent)
	{
		DamageCheckComponent->EndCheck();
	}
}

