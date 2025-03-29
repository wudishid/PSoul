// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SoulUserWidget.h"
#include "EquipmentSlot.generated.h"

enum class EEquipmentType : uint8;
/**
 * 
 */
UCLASS(Abstract)
class PSOUL_API UEquipmentSlot : public USoulUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "EquipmentSlot")
	EEquipmentType EquipmentType;
};
