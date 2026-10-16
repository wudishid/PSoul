// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANF_SpawnProjectile.generated.h"

class AProjectileBase;
/**
 * Launches a projectile from the notify owner.
 *
 * The projectile is taken from the world's UProjectilePoolSubsystem rather than spawned outright, so
 * repeated shots of the same class stop allocating once the pool is warm.
 */
UCLASS()
class PSOUL_API UANF_SpawnProjectile : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	/** Projectile to launch. Instances of this class are recycled through the world's projectile pool. */
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;
};
