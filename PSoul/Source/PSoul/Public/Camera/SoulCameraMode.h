// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoulCameraMode.generated.h"


class USoulCameraComponent;

USTRUCT()
struct FSoulCameraModeView
{
	GENERATED_BODY()

	FSoulCameraModeView() : Location(FVector::Zero()), Rotation(FRotator::ZeroRotator), ControlRotation(FRotator::ZeroRotator), FieldOfView(80.f)
	{
	}
	
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};


UCLASS()
class PSOUL_API USoulCameraMode : public UObject
{
	GENERATED_BODY()

public:
	USoulCameraMode();
	USoulCameraComponent* GetCameraComponent() const;
	virtual UWorld* GetWorld() const override;
	AActor* GetTargetActor() const;
	virtual void OnActivation(){}
	virtual void OnDeactivation(){}
	
	void UpdateCameraMode(float DeltaTime);
	const FSoulCameraModeView& GetCamereModeView() const {return View;}
	
protected:
	virtual void UpdateView(float DeltaTime);
	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;
	
protected:
	// The horizontal field of view (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView = 80.f;

	// Minimum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin = -89.f;

	// Maximum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax = 89.f;
	
	FSoulCameraModeView View;
};
