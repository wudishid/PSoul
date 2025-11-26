// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GAS/GameplayEffect/SoulGameplayEffect_Damage.h"
#include "ANFS_DamageCheck.generated.h"

class UDamageCheckComponent;

/**
 * 
 */
UCLASS()
class PSOUL_API UANFS_DamageCheck : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
protected:
	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	bool bForceUseBoxTrace = false;
	
	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	FDamageInfo DamageInfo;
};
