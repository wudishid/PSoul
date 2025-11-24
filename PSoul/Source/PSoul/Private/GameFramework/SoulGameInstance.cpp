// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/SoulGameInstance.h"


void USoulGameInstance::Init()
{
	Super::Init();

	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "USoulGameInstance::Init");
	
	FParse::Value(FCommandLine::Get(), TEXT("-EnterGameMap="), EnterGameMap);
}

void USoulGameInstance::Shutdown()
{
	Super::Shutdown();
	
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "USoulGameInstance::Shutdown");
}
