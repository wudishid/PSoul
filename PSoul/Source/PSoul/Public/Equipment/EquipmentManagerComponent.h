// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentManagerComponent.generated.h"


class UInventoryManagerComponent;
struct FInventoryItemInfo;
enum class EEquipmentType : uint8;
class AEquipmentInstance;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquip, EEquipmentType, AEquipmentInstance*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnEquip, EEquipmentType);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

	UFUNCTION(Server, Reliable)
	void Equip(TSubclassOf<AEquipmentInstance> EquipmentClass);

	UFUNCTION(Server, Reliable)
	void UnEquip(TSubclassOf<AEquipmentInstance> EquipmentClass);

	UFUNCTION(Server, Reliable)
	void Drop(EEquipmentType InEquipmentType);
	
	bool GetWearedEquipmentInof(EEquipmentType InEquipmentType, FInventoryItemInfo& OutItemInfo);
	
	AEquipmentInstance* GetEquipmentInstance(EEquipmentType InEquipmentType);
	
	FOnEquip OnEquip;
	FOnUnEquip OnUnEquip;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Weapon();

	UFUNCTION()
	void OnRep_Armor();

	UFUNCTION()
	void OnRep_Ring();
	
	UPROPERTY(ReplicatedUsing = OnRep_Weapon)
	AEquipmentInstance* Weapon;
	UPROPERTY(ReplicatedUsing = OnRep_Armor)
	AEquipmentInstance* Armor;
	UPROPERTY(ReplicatedUsing = OnRep_Ring)
	AEquipmentInstance* Ring;

public:
};
