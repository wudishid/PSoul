// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/SoulCameraComponent.h"
#include "Camera/SoulCameraMode.h"
#include "Camera/SoulCameraMode_ThirdPerson.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

USoulCameraComponent::USoulCameraComponent()
{
	// Camera modes own the framing, so the component must not also apply pawn control rotation.
	bUsePawnControlRotation = false;

	DefaultCameraModeClass = USoulCameraMode_ThirdPerson::StaticClass();
}

void USoulCameraComponent::OnRegister()
{
	Super::OnRegister();

	// The class default object must never instance camera modes.
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// Camera modes are per-instance runtime objects. Anything inherited from an archetype belongs to a
	// different outer, so discard it rather than risk driving this camera from another component.
	if (CurrentCameraMode && CurrentCameraMode->GetOuter() != this)
	{
		CurrentCameraMode = nullptr;
	}

	CameraModeInstances.RemoveAll([this](const TObjectPtr<USoulCameraMode>& CameraModeInstance)
	{
		return !CameraModeInstance || CameraModeInstance->GetOuter() != this;
	});

	if (!CurrentCameraMode)
	{
		TSubclassOf<USoulCameraMode> CameraModeClass;
		if (DefaultCameraModeClass)
		{
			CameraModeClass = DefaultCameraModeClass;
		}
		else
		{
			CameraModeClass = USoulCameraMode_ThirdPerson::StaticClass();
		}
		SetCameraMode(CameraModeClass);
	}
}

void USoulCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	// Let the base component fill in projection mode, aspect ratio, ortho planes and the post process
	// settings first, then replace only the transform and field of view with the mode's framing.
	Super::GetCameraView(DeltaTime, DesiredView);

	// Normally set up in OnRegister; this covers components that were never registered through it.
	if (!CurrentCameraMode)
	{
		
		TSubclassOf<USoulCameraMode> CameraModeClass;
		if (DefaultCameraModeClass)
		{
			CameraModeClass = DefaultCameraModeClass;
		}
		else
		{
			CameraModeClass = USoulCameraMode_ThirdPerson::StaticClass();
		}
		SetCameraMode(CameraModeClass);
		if (!CurrentCameraMode)
		{
			return;
		}
	}

	CurrentCameraMode->UpdateCameraMode(DeltaTime);

	if (bHasProducedView)
	{
		// Ease from what we presented last frame toward the active mode's view.
		View.Blend(CurrentCameraMode->GetCameraModeView(), CurrentCameraMode->GetBlendWeight());
	}
	else
	{
		// Nothing to blend from yet. Adopt the mode's view outright so the camera does not fly in
		// from a default-constructed view sitting at the world origin.
		View = CurrentCameraMode->GetCameraModeView();
		bHasProducedView = true;
	}

	DesiredView.Location = View.Location;
	DesiredView.Rotation = View.Rotation;
	DesiredView.FOV = View.FieldOfView;

	ApplyControlRotationToOwner();
}

AActor* USoulCameraComponent::GetTargetActor() const
{
	return GetOwner();
}

void USoulCameraComponent::SetCameraMode(TSubclassOf<USoulCameraMode> InCameraModeClass)
{
	if (!InCameraModeClass)
	{
		return;
	}

	// Already active. Bail out so we do not restart the blend-in.
	if (CurrentCameraMode && CurrentCameraMode->GetClass() == InCameraModeClass)
	{
		return;
	}

	USoulCameraMode* CameraModeInstance = GetCameraModeInstance(InCameraModeClass);
	if (!CameraModeInstance)
	{
		CameraModeInstance = NewObject<USoulCameraMode>(this, InCameraModeClass);
		if (!CameraModeInstance)
		{
			return;
		}

		CameraModeInstances.Add(CameraModeInstance);
	}

	if (CurrentCameraMode)
	{
		CurrentCameraMode->OnDeactivation();
	}

	CurrentCameraMode = CameraModeInstance;
	CurrentCameraMode->OnActivation();
}

USoulCameraMode* USoulCameraComponent::GetCameraModeInstance(TSubclassOf<USoulCameraMode> InCameraModeClass)
{
	for (const TObjectPtr<USoulCameraMode>& CameraModeInstance : CameraModeInstances)
	{
		if (CameraModeInstance && CameraModeInstance->GetClass() == InCameraModeClass)
		{
			return CameraModeInstance.Get();
		}
	}

	return nullptr;
}

void USoulCameraComponent::ApplyControlRotationToOwner()
{
	if (!bAllowControlRotationWriteback || !CurrentCameraMode || !CurrentCameraMode->ShouldApplyControlRotationToOwner())
	{
		return;
	}

	const APawn* Pawn = Cast<APawn>(GetTargetActor());
	if (!Pawn)
	{
		return;
	}

	// Only the locally controlled copy should be driven; remote copies follow input from their own client.
	AController* Controller = Pawn->GetController();
	if (Controller && Controller->IsLocalController())
	{
		Controller->SetControlRotation(View.ControlRotation);
	}
}
