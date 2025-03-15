// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulCharacterBase.generated.h"

class USoulCharacterSet;
class UCharacterAttributeComponent;
class USoulAbilitySystemComponent;

UCLASS()
class PSOUL_API ASoulCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoulCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	USoulAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeComp")
	UCharacterAttributeComponent* AttributeComponent;
	
};
