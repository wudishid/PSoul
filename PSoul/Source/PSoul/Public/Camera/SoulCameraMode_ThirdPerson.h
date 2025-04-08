// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCameraMode.h"
#include "SoulCameraMode_ThirdPerson.generated.h"

/**
 * 
 */
UCLASS()
class PSOUL_API USoulCameraMode_ThirdPerson : public USoulCameraMode
{
	GENERATED_BODY()

protected:
	virtual void UpdateView(float DeltaTime) override;
};
