// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "SoulCharacterBase.generated.h"

struct FGameplayAttribute;
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

UENUM(BlueprintType)
enum class ECharacterTeam : uint8
{
	AI UMETA(DisplayName = "AI"),
	Player UMETA(DisplayName = "玩家")
};


UCLASS()
class PSOUL_API ASoulCharacterBase : public ACharacter, public IGenericTeamAgentInterface 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoulCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(Server, Reliable)
	virtual void HandleKill(AActor* InKilled);

	UFUNCTION(BlueprintNativeEvent, Category = "SoulCharacterBase")
	void OnDeath();
	
	virtual FRotator GetDesiredRotation() const;

	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleAttributeChanged(FGameplayAttribute Attribute, float CurrentValue, float OldValue);
	
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
	TObjectPtr<UDamageCheckComponent> DamageCheckComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	ECharacterTeam CharacterTeam;
	
};
