// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulCharacterBase.generated.h"

class UMotionWarpingComponent;
class AEquipmentInstance;
enum class EEquipmentType : uint8;
class UEquipmentManagerComponent;
class UInventoryManagerComponent;
class UDamageCheckComponent;
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
	virtual void HandleKill(AActor* InKilled);

	UFUNCTION()
	virtual void HandleDeath();

	UFUNCTION(BlueprintCallable)
	virtual void FinishDeath();
	
	virtual FRotator GetDesiredRotation() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	FORCEINLINE class USoulAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	USoulAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeComp")
	UCharacterAttributeComponent* AttributeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeComp")
	UWidgetComponent* HealthBarComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageCheckComp")
	UDamageCheckComponent* DamageCheckComponent;
};
