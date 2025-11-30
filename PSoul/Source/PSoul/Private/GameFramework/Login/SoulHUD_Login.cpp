// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Login/SoulHUD_Login.h"
#include "UI/Login/MainMenu.h"

void ASoulHUD_Login::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(LoginPanelClass))
	{
		UMainMenu* LoginPanel = CreateWidget<UMainMenu>(GetOwningPlayerController(), LoginPanelClass);
		LoginPanel->AddToViewport();
	}
}

