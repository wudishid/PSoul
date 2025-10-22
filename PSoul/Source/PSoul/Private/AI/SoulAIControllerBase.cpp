// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SoulAIControllerBase.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"


// Sets default values
ASoulAIControllerBase::ASoulAIControllerBase()
{
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

void ASoulAIControllerBase::InitPlayerState()
{
	Super::InitPlayerState();
}


