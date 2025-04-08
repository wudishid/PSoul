// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCameraMode.h"
#include "Camera/CameraComponent.h"
#include "SoulCameraComponent.generated.h"

class USoulCameraMode;
/**
 * 
 */
UCLASS()
class PSOUL_API USoulCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	USoulCameraComponent();
	static USoulCameraComponent* FindSoulCameraComponent(const AActor* InActor) { return InActor ? InActor->FindComponentByClass<USoulCameraComponent>() : nullptr; }
	virtual AActor* GetTargetActor() const;
	void SetCameraMode(TSubclassOf<USoulCameraMode> InCameraModeClass);
	USoulCameraMode* GetCameraModeInstance(TSubclassOf<USoulCameraMode> InCameraModeClass);
protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	
protected:
	UPROPERTY()
	TObjectPtr<USoulCameraMode> CurrentCameraMode;

	UPROPERTY()
	TArray<USoulCameraMode*> CameraModeInstances;

private:
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USoulCameraMode> DefaultCameraModeClass = USoulCameraMode::StaticClass();
};
