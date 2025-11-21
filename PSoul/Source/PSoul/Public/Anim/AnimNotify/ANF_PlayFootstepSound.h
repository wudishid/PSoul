// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANF_PlayFootstepSound.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API UANF_PlayFootstepSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "FootStepSound")
	TObjectPtr<USoundBase> Sound;
};
