// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerController.h"
#include "SoulPlayerController_Game.generated.h"

class UQuickSkillManager;
class USoulInputConfig;
enum class ESoulCharacterTeam : uint8;
class USoulAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PSOUL_API ASoulPlayerController_Game : public ASoulPlayerController
{
	GENERATED_BODY()

public:
	ASoulPlayerController_Game(const FObjectInitializer& ObjectInitializer);
	
	USoulAbilitySystemComponent* GetAbilitySystemComponent() const;
	void HandlePlayerDeath();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<USoulInputConfig> InputConfig;
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UFUNCTION(Server, Reliable)
	void InitSoulPlayerState();
	
	void ToggleShowInventoryPanel();
	void ToggleShowSkillTreePanel();

	void PressSkill1();
	void PressSkill2();
	void PressSkill3();
	void PressSkill4();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuickSkillManager")
	TObjectPtr<UQuickSkillManager> QuickSkillManager;
};
