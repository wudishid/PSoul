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

	//默认最大移动速度
	UPROPERTY(EditAnywhere, Category = "SoulCharacterMovement")
	float DefaultMaxSpeed = 600.f;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	USoulAbilitySystemComponent* ASC = nullptr;
};
