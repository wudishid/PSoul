// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributeComponent.generated.h"

class USoulAbilitySystemComponent;
class USoulCharacterSet;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttrributeChanged, FGameplayAttribute, GameplayAttribute, float, CurrentValue, float, OldValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API UCharacterAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void HandleAttributeChanged(FGameplayAttribute Attribute, float CurrentValue, float OldValue);
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttrributeChanged OnAttributeChanged;
	
	void InitWithAbilitySystemComponent(USoulAbilitySystemComponent* InASC);
protected:
	UPROPERTY()
	const USoulCharacterSet* CharacterSet;

	UPROPERTY()
	USoulAbilitySystemComponent* ASC;
};
