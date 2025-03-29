// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentInstance.h"
#include "Equipment_Weapon.generated.h"

UCLASS(Abstract)
class PSOUL_API AEquipment_Weapon : public AEquipmentInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEquipment_Weapon();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Equip() override;
	virtual void UnEquip() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponMesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponMesh")
	FName SocketName;
};
