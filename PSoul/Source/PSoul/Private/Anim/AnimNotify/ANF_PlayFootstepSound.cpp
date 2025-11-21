// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotify/ANF_PlayFootstepSound.h"

#include "Kismet/GameplayStatics.h"

void UANF_PlayFootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UWorld* World = MeshComp->GetWorld();
	FActorSpawnParameters Sp;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
	// 1. 定义射线起点和终点（垂直向下）
	FVector StartLocation = MeshComp->GetComponentLocation();
	FVector EndLocation = StartLocation + FVector(0, 0, -1000);

	// 2. 执行射线检测检测地面
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		TraceParams
	);
		
	if (bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(World, Sound, HitResult.Location);
	}
}
