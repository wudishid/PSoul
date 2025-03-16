// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Login/SoulHUD_Login.h"
#include "UI/Login/LoginPanel.h"

void ASoulHUD_Login::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(LoginPanelClass))
	{
		ULoginPanel* LoginPanel = CreateWidget<ULoginPanel>(GetOwningPlayerController(), LoginPanelClass);
		LoginPanel->AddToViewport();
	}
}

