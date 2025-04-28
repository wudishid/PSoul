// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentInstance.h"
#include "Components/ActorComponent.h"
#include "EquipmentManagerComponent.generated.h"


class UInventoryManagerComponent;
struct FInventoryItemInfo;
enum class EEquipmentType : uint8;
class AEquipmentInstance;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquip, EEquipmentType, AEquipmentInstance*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnEquip, EEquipmentType);



USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY()

	FEquipmentSlot() : EquipmentType(EEquipmentType::Weapon), EquipmentInstance(nullptr)
	{
	}

	FEquipmentSlot(EEquipmentType InEquipmentType, AEquipmentInstance* InEquipmentInstance) : EquipmentType(InEquipmentType), EquipmentInstance(InEquipmentInstance)
	{
	}

	//不标记为UPROPERTY就不会被复制
	UPROPERTY()
	EEquipmentType EquipmentType;

	UPROPERTY()
	AEquipmentInstance* EquipmentInstance;
};


USTRUCT(BlueprintType)
struct FEquipmentSlotList
{
	GENERATED_BODY()

	void SetEquipmentSlot(EEquipmentType InEquipmentType, AEquipmentInstance* InEquipmentInstance);
	AEquipmentInstance* GetEquipmentByType(EEquipmentType InEquipmentType);
	
	void AddEntry(FEquipmentSlot Entry);
	
	UPROPERTY()
	TArray<FEquipmentSlot> Slots;
	
};




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
	void InitEquipmentSlotList();
	void HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* InEquipmentInstance);
	void HandleUnEquip(EEquipmentType InEquipmentType);

	UFUNCTION()
	void OnRep_EquipmentSlotList(const FEquipmentSlotList& OldEquipmentSlotList);
	
	UPROPERTY(ReplicatedUsing = OnRep_EquipmentSlotList)
	FEquipmentSlotList EquipmentSlotList;
public:
};
