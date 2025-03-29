// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipmentInstance.generated.h"


enum class EEquipmentType : uint8;

UCLASS(Abstract)
class PSOUL_API AEquipmentInstance : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AEquipmentInstance();
	virtual void Equip();
	virtual void UnEquip();
	EEquipmentType GetEquipmentType() const { return EquipmentType; };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "EquipmentInstance")
	EEquipmentType EquipmentType;
};
