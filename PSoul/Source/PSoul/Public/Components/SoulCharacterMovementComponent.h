// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SoulCharacterMovementComponent.generated.h"


struct FGameplayTag;
class USoulAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API USoulCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USoulCharacterMovementComponent();
	virtual float GetMaxSpeed() const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	USoulAbilitySystemComponent* ASC = nullptr;
private:
	void HandleGameplayTagChanged(const FGameplayTag GameplayTag, int32 Count);
};
