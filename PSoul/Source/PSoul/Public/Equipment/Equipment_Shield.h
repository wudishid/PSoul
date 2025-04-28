// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "EquipmentInstance.h"
#include "GameplayAbilitySpecHandle.h"
#include "Equipment_Shield.generated.h"


class UGameplayAbility_Block;

UCLASS()
class PSOUL_API AEquipment_Shield : public AEquipmentInstance
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AEquipment_Shield();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Equip() override;
	virtual void UnEquip() override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Shield")
	UStaticMeshComponent* ShieldMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	TSubclassOf<UGameplayAbility_Block> BlockAbilityToGive;

	FGameplayAbilitySpecHandle CombAttackAbilitySpecHandle;
	
};
