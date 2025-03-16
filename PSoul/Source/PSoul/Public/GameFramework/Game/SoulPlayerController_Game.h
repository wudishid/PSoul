// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerController.h"
#include "SoulPlayerController_Game.generated.h"

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
	USoulAbilitySystemComponent* GetAbilitySystemComponent() const;
	void HandlePlayerDeath();
	void HandlePlayerKill();
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void AcknowledgePossession(class APawn* P) override;

	UFUNCTION()
	void OnRep_Team();

	UFUNCTION(Server, Reliable)
	void InitSoulPlayerState();
	
	UPROPERTY(ReplicatedUsing = OnRep_Team)
	ESoulCharacterTeam Team;
private:
	UFUNCTION(Server, Reliable)
	void SetTeam(ESoulCharacterTeam InTeam);
	
};
