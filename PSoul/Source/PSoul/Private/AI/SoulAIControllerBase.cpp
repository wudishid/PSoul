// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/SoulAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/SoulCharacterBase.h"


// Sets default values
ASoulAIControllerBase::ASoulAIControllerBase()
{
}

FGenericTeamId ASoulAIControllerBase::GetGenericTeamId() const
{
	if (ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetPawn()))
	{
		return SoulCharacter->GetGenericTeamId();
	}
	
	return Super::GetGenericTeamId();
}

AActor* ASoulAIControllerBase::GetCurrentAttackTarget() const
{
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
}

// Called when the game starts or when spawned
void ASoulAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

