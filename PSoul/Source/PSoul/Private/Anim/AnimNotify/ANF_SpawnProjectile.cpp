// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/ANF_SpawnProjectile.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Misc/Projectile/ProjectileBase.h"
#include "Subsystem/ProjectilePoolSubsystem.h"

void UANF_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!ProjectileClass)
	{
		return;
	}

	AActor* OwnerActor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!OwnerActor || !OwnerActor->HasAuthority())
	{
		return;
	}

	UWorld* World = OwnerActor->GetWorld();
	if (!World)
	{
		return;
	}

	const FTransform SpawnTransform(OwnerActor->GetActorRotation(), OwnerActor->GetActorLocation());
	APawn* OwnerPawn = Cast<APawn>(OwnerActor);

	// Reuse a parked projectile instead of spawning one. Only the first few shots of a class allocate.
	if (UProjectilePoolSubsystem* ProjectilePool = World->GetSubsystem<UProjectilePoolSubsystem>())
	{
		ProjectilePool->AcquireProjectile(ProjectileClass, SpawnTransform, OwnerActor, OwnerPawn);
		return;
	}

	// No pool in this world (unsupported world type): fall back to a plain spawn so nothing regresses.
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerPawn;
	World->SpawnActor<AProjectileBase>(ProjectileClass, OwnerActor->GetActorLocation(), OwnerActor->GetActorRotation(), SpawnParams);
}
