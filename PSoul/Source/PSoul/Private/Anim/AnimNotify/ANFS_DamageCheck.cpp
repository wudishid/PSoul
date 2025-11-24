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
					DamageCheckComponent->ResetCheck();
					DamageCheckComponent->SetDamageInfo(DamageInfo);
					DamageCheckComponent->CheckDamage(bForceUseBoxTrace);
				}
			}
		}
	}
}

void UANFS_DamageCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (OwnerActor->HasAuthority())
		{
			if(UDamageCheckComponent* DamageCheckComponent = OwnerActor->FindComponentByClass<UDamageCheckComponent>())
			{
				if(DamageCheckComponent)
				{
					DamageCheckComponent->CheckDamage(bForceUseBoxTrace);
				}
			}
		}
	}

	
}


