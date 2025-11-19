// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim/AnimNotify/ANFS_DamageCheck.h"
#include "Components/DamageCheckComponent.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/GameplayEffect/DamageGameplayEffectComponent.h"
#include "Math/UnitConversion.h"


void UANFS_DamageCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (OwnerActor->HasAuthority())
		{
			if(UDamageCheckComponent* DamageCheckComponent = OwnerActor->FindComponentByClass<UDamageCheckComponent>())
			{
				if(DamageCheckComponent)
				{
					DamageCheckComponent->SetDamageInfo(DamageInfo);
					DamageCheckComponent->StartCheck();
				}
			}
		}
	}
}

void UANFS_DamageCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (OwnerActor->HasAuthority())
		{
			if (UDamageCheckComponent* DamageCheckComponent = OwnerActor->FindComponentByClass<UDamageCheckComponent>())
			{
				DamageCheckComponent->EndCheck();
			}
		}
	}
}

