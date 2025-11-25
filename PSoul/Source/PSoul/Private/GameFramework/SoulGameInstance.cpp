// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/SoulGameInstance.h"


void USoulGameInstance::Init()
{
	Super::Init();
	FParse::Value(FCommandLine::Get(), TEXT("-EnterGameMap="), EnterGameMap);
}

void USoulGameInstance::Shutdown()
{
	Super::Shutdown();
}
