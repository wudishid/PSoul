// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulCharacterBase.generated.h"

class UWidgetComponent;
class USoulCharacterSet;
class UCharacterAttributeComponent;
class USoulAbilitySystemComponent;

UCLASS()
class PSOUL_API ASoulCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoulCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UFUNCTION(Server, Reliable)
	virtual void HandleKill();

	UFUNCTION()
	virtual void HandleDeath();

	UFUNCTION(BlueprintCallable)
	virtual void FinishDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyRestarted() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void NetMulticastHandleDeath();
public:
	FORCEINLINE class USoulAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	USoulAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeComp")
	UCharacterAttributeComponent* AttributeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeComp")
	UWidgetComponent* HealthBarComp;
	
};
