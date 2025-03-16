// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributeComponent.generated.h"

class USoulAbilitySystemComponent;
class USoulCharacterSet;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttrributeChanged, FGameplayAttribute, GameplayAttribute, float, CurrentValue, float, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

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

	
	void HandleCharacterDeath(AActor* InCauser);

	UFUNCTION(Client, Reliable)
	void Client_SendDeathEvent();
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttrributeChanged OnAttributeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeath OnCharacterDeath;
	
	void InitWithAbilitySystemComponent(USoulAbilitySystemComponent* InASC);
	
	float GetAttributeValue(FGameplayAttribute Attribute) const;
	
protected:
	UPROPERTY()
	const USoulCharacterSet* CharacterSet;

	UPROPERTY()
	USoulAbilitySystemComponent* ASC;
};
