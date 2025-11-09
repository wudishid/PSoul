// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "QuickSkillManager.generated.h"


class USoulAbilitySystemComponent;
class USkillTreeManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuickSkillChanged, FGameplayTag);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuickSkillReleased, FGameplayTag);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API UQuickSkillManager : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UQuickSkillManager();

	FOnQuickSkillChanged OnQuickSkillChanged;
	FOnQuickSkillReleased OnQuickSkillReleased;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	void OnSetPawn(APawn* InPawn);
	void PressSkill(FGameplayTag InSkillInputTag);
	
	void SetQuickSkill(FGameplayTag InSkillInputTag, FName InSkillID);
	const TMap<FGameplayTag, FName>& GetQuickSkillSlots() const { return QuickSkillSlots; }

	float GetQuickSkillCooldownRemainTime(FGameplayTag InSkillInputTag) const;
private:
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="QuickSkillManager")
	TMap<FGameplayTag, FName> QuickSkillSlots;

	UPROPERTY()
	TObjectPtr<USkillTreeManager> SkillTreemanager;
	
	UPROPERTY()
	TObjectPtr<USoulAbilitySystemComponent> ASC;
};
