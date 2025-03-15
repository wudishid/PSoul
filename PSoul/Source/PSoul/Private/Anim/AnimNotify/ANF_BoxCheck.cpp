// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/ANF_BoxCheck.h"

#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UANF_BoxCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

#if WITH_SERVER_CODE

	if (ASoulCharacterBase* OwnerCharacter = Cast<ASoulCharacterBase>(MeshComp->GetOwner()))
	{
		FVector StartPos = OwnerCharacter->GetActorLocation();
		FVector EndPos = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector();
		TArray<AActor*> ActorsToIgnore{OwnerCharacter};
		TArray<FHitResult> Hits;
		UKismetSystemLibrary::BoxTraceMultiForObjects(OwnerCharacter->GetWorld(), StartPos, EndPos, BoxHalfSize,
		                                              OwnerCharacter->GetActorRotation(), TraceObjectType, false,
		                                              ActorsToIgnore, DrawDebugTraceType, Hits, true);

		if (!Hits.IsEmpty())
		{
			TArray<AActor*> HitActors;
			for(const FHitResult& Hit : Hits)
			{
				if(AActor* HitActor = Hit.GetActor())
				{
					HitActors.AddUnique(HitActor);
				}
			}

			if(!HitActors.IsEmpty())
			{
				USoulAbilitySystemComponent* CauserASC = OwnerCharacter->GetAbilitySystemComponent();
				if(!CauserASC) return;
				for(AActor* HitActor : HitActors)
				{
					USoulAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<USoulAbilitySystemComponent>();
					if(!TargetASC) continue;
					UGameplayEffect* GameplayEffect = EffectToApply.GetDefaultObject();
					CauserASC->ApplyGameplayEffectToTarget(GameplayEffect , TargetASC, 1);
				}
			}
		}
	}

#endif
}
