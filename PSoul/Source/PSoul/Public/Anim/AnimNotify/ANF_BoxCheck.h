// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ANF_BoxCheck.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PSOUL_API UANF_BoxCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BoxCheck")
	TSubclassOf<UGameplayEffect> EffectToApply;
	
	UPROPERTY(EditAnywhere, Category = "BoxCheck")
	FVector BoxHalfSize = FVector(50, 50, 50);

	UPROPERTY(EditAnywhere, Category = "BoxCheck")
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectType;

	UPROPERTY(EditAnywhere, Category = "BoxCheck")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceType;
};
