// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/ANF_SpawnProjectile.h"

#include "Misc/Projectile/ProjectileBase.h"

void UANF_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (OwnerActor->HasAuthority())
		{
			UWorld* World = OwnerActor->GetWorld();
			if (!World) return;
			FActorSpawnParameters sp;
			sp.Owner = OwnerActor;
			World->SpawnActor<AProjectileBase>(ProjectileClass, OwnerActor->GetActorLocation(), OwnerActor->GetActorRotation(), sp);
		}
	}
}
