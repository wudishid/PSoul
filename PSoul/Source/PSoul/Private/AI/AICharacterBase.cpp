// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/AICharacterBase.h"
#include "AI/SoulAIControllerBase.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AAICharacterBase::AAICharacterBase()
{
}

FRotator AAICharacterBase::GetDesiredRotation() const
{
	if (ASoulAIControllerBase* AIController = Cast<ASoulAIControllerBase>(GetController()))
	{
		if (AActor* AttackTarget = AIController->GetCurrentAttackTarget())
		{
			return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
		}
	}
	
	return Super::GetDesiredRotation();
}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
}



