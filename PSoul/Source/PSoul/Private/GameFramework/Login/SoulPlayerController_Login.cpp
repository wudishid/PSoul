// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Login/SoulPlayerController_Login.h"

#include "GameFramework/Login/SoulHUD_Login.h"

void ASoulPlayerController_Login::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	FInputModeUIOnly ModeUIOnly;
	SetShowMouseCursor(true);
	SetInputMode(ModeUIOnly);
}
