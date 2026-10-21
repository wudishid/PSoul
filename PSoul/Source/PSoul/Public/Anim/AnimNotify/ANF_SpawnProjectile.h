// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANF_SpawnProjectile.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class PSOUL_API UANF_SpawnProjectile : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "FootStepSound")
	TSubclassOf<AProjectileBase> ProjectileClass;
};
