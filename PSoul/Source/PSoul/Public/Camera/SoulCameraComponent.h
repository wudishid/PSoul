// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCameraMode.h"
#include "Camera/CameraComponent.h"
#include "SoulCameraComponent.generated.h"

class USoulCameraMode;

/**
 * Camera component that delegates all framing to a swappable USoulCameraMode.
 *
 * The active mode produces a FSoulCameraModeView each frame; this component blends that with the view
 * it presented last frame, so activating a different mode eases into the new framing rather than
 * snapping. The component's own transform is only an attachment point - it does not drive the view.
 */
UCLASS(ClassGroup = (Camera), meta = (BlueprintSpawnableComponent))
class PSOUL_API USoulCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	USoulCameraComponent();

	static USoulCameraComponent* FindSoulCameraComponent(const AActor* InActor) { return InActor ? InActor->FindComponentByClass<USoulCameraComponent>() : nullptr; }

	virtual AActor* GetTargetActor() const;

	/**
	 * Makes InCameraModeClass the active mode, blending away from whatever was previously active.
	 * Instances are cached per class, so switching back and forth does not reallocate.
	 * Does nothing when the requested class is already active.
	 */
	void SetCameraMode(TSubclassOf<USoulCameraMode> InCameraModeClass);

	USoulCameraMode* GetCameraModeInstance(TSubclassOf<USoulCameraMode> InCameraModeClass);

	UFUNCTION(BlueprintPure, Category = "Camera")
	USoulCameraMode* GetCurrentCameraMode() const { return CurrentCameraMode.Get(); }

	/** The blended view this component presented most recently. */
	const FSoulCameraModeView& GetCurrentView() const { return View; }

protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	/** Pushes the blended ControlRotation onto the locally controlled owner, when the mode asks for it. */
	void ApplyControlRotationToOwner();

protected:
	UPROPERTY(Transient)
	TObjectPtr<USoulCameraMode> CurrentCameraMode;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USoulCameraMode>> CameraModeInstances;

	// View carried between frames so that activating a new mode eases rather than snaps.
	FSoulCameraModeView View;

	// False until the first view has been produced; the very first frame adopts the mode's view outright
	// instead of blending in from a default-constructed view at the world origin.
	bool bHasProducedView = false;

	// When false the component never writes back to the controller, whatever the active mode requests.
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	bool bAllowControlRotationWriteback = true;

private:
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USoulCameraMode> DefaultCameraModeClass;
};
