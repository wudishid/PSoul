// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulHUD.h"
#include "SoulHUD_Game.generated.h"

class UPlayerPanel;
/**
 * 
 */
UCLASS()
class PSOUL_API ASoulHUD_Game : public ASoulHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void InitHUD();
	void SetShowInventoryPanel(bool bShow);
	bool IsShowingInventoryPanel() const;
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerPanel> PlayerPanelClass;

	UPROPERTY()
	TObjectPtr<UPlayerPanel> PlayerPanel;
};
