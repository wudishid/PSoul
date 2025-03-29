// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentManagerComponent.generated.h"


class AEquipmentInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

	UFUNCTION(Server, Reliable)
	void WearEquipment(TSubclassOf<AEquipmentInstance> EquipmentClass);

	UFUNCTION(Server, Reliable)
	void RemoveEquipment(TSubclassOf<AEquipmentInstance> EquipmentClass);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY()
	AEquipmentInstance* Weapon;
	UPROPERTY()
	AEquipmentInstance* Armor;
	UPROPERTY()
	AEquipmentInstance* Ring;
public:
};
