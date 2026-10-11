// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoulCameraMode.generated.h"


class USoulCameraComponent;

/**
 * The framing produced by a camera mode for a single frame.
 *
 * The owning USoulCameraComponent keeps the view it presented last frame and blends it toward the
 * active mode's view, so activating a different mode eases into the new framing instead of snapping.
 */
USTRUCT(BlueprintType)
struct PSOUL_API FSoulCameraModeView
{
	GENERATED_BODY()

	FSoulCameraModeView();

	/**
	 * Blend this view toward Other.
	 *
	 * @param OtherWeight Blend weight in [0,1]. 0 leaves this view untouched, 1 adopts Other outright.
	 */
	void Blend(const FSoulCameraModeView& Other, float OtherWeight);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "View")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "View")
	FRotator Rotation;

	/** Rotation the controller should adopt, so the character body follows the camera. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "View")
	FRotator ControlRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "View")
	float FieldOfView;
};


/**
 * Base class for a camera behaviour.
 *
 * A mode is a plain UObject owned by the camera component it drives. Subclasses override
 * UpdateView() - and optionally GetPivotLocation()/GetPivotRotation() - to produce one
 * FSoulCameraModeView per frame. Modes never write to the component's transform directly.
 */
UCLASS(BlueprintType, Blueprintable)
class PSOUL_API USoulCameraMode : public UObject
{
	GENERATED_BODY()

public:
	USoulCameraMode();

	USoulCameraComponent* GetCameraComponent() const;
	virtual UWorld* GetWorld() const override;
	AActor* GetTargetActor() const;

	/** Called when this mode becomes the active mode. */
	virtual void OnActivation();

	/** Called when this mode stops being the active mode. */
	virtual void OnDeactivation();

	/** Recomputes the view for this frame and advances the blend-in weight. */
	void UpdateCameraMode(float DeltaTime);

	const FSoulCameraModeView& GetCameraModeView() const { return View; }

	/** Blend-in progress in [0,1]; the owning camera uses it to ease away from the previous view. */
	float GetBlendWeight() const { return BlendWeight; }

	/** True when the owning camera should push View.ControlRotation onto the locally controlled owner. */
	bool ShouldApplyControlRotationToOwner() const { return bApplyControlRotationToOwner; }

	/** Field of view this mode presents, in degrees. */
	virtual float GetFieldOfView() const { return FieldOfView; }

protected:
	virtual void UpdateView(float DeltaTime);
	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;

	/** Advances BlendWeight toward 1 over BlendTime. */
	void UpdateBlending(float DeltaTime);

protected:
	// The horizontal field of view (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView = 80.f;

	// Minimum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin = -89.f;

	// Maximum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax = 89.f;

	// Seconds spent easing from the previous view into this mode's view. 0 snaps immediately.
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float BlendTime = 0.25f;

	// When true the owning camera keeps the controller's rotation in sync with ControlRotation.
	UPROPERTY(EditDefaultsOnly, Category = "View")
	bool bApplyControlRotationToOwner = false;

	FSoulCameraModeView View;

private:
	// Blend-in progress for this mode, driven by UpdateBlending().
	float BlendWeight = 1.f;
};
