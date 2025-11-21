// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SoulPlayerController.h"
#include "SoulPlayerController_Game.generated.h"

struct FGameplayTag;
struct FInputActionValue;
class UInputMappingContext;
class UQuickSkillManager;
class USoulInputConfig;
enum class ESoulCharacterTeam : uint8;
class USoulAbilitySystemComponent;


UENUM(BlueprintType)
enum class EInputMappingContextMode : uint8
{
	Default					UMETA(DisplayName = "Default"),
	Inventory				UMETA(DisplayName = "Inventory"),
	SkillTree				UMETA(DisplayName = "SkillTree")
};



UCLASS()
class PSOUL_API ASoulPlayerController_Game : public ASoulPlayerController
{
	GENERATED_BODY()

public:
	ASoulPlayerController_Game(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SoulPlayerController_Game")
	void Rebirth();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<EInputMappingContextMode, TObjectPtr<UInputMappingContext>> InputMappingContexts;
	
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
	
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	void ToggleShowInventoryPanel();
	void ToggleShowSkillTreePanel();

	void PressSkill1();
	void PressSkill2();
	void PressSkill3();
	void PressSkill4();

	void SetInputMappingContextMode(EInputMappingContextMode InContextMode);
	void SetInputModeGame();
	void SetInputModeUI(EInputMappingContextMode InContextMode);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuickSkillManager")
	TObjectPtr<UQuickSkillManager> QuickSkillManager;

	TWeakObjectPtr<USoulAbilitySystemComponent> ASC;
};
