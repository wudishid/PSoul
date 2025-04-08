// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/SoulCameraComponent.h"
#include "Camera/SoulCameraMode.h"

void USoulCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CurrentCameraMode);
	CurrentCameraMode->UpdateCameraMode(DeltaTime);
	FSoulCameraModeView CameraModeView = CurrentCameraMode->GetCamereModeView();

	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
}

USoulCameraComponent::USoulCameraComponent()
{
}

AActor* USoulCameraComponent::GetTargetActor() const
{
	return GetOwner();
}

void USoulCameraComponent::SetCameraMode(TSubclassOf<USoulCameraMode> InCameraModeClass)
{
	USoulCameraMode* CameraModeInstance = GetCameraModeInstance(InCameraModeClass);
	if (!CameraModeInstance)
	{
		CameraModeInstance = NewObject<USoulCameraMode>(this, InCameraModeClass);
	}
	CameraModeInstances.AddUnique(CameraModeInstance);
	CurrentCameraMode = CameraModeInstance;
}

USoulCameraMode* USoulCameraComponent::GetCameraModeInstance(TSubclassOf<USoulCameraMode> InCameraModeClass)
{
	for(USoulCameraMode* CameraModeInstance : CameraModeInstances)
	{
		if ((CameraModeInstance != nullptr) && (CameraModeInstance->GetClass() == InCameraModeClass))
		{
			return CameraModeInstance;
		}
	}

	return nullptr;
}

void USoulCameraComponent::OnRegister()
{
	Super::OnRegister();

	if(!CurrentCameraMode)
	{
		CurrentCameraMode = NewObject<USoulCameraMode>(this, DefaultCameraModeClass);
	}
}
