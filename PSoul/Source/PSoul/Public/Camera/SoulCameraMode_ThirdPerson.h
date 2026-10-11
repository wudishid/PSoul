// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Camera/SoulCameraMode.h"
#include "SoulCameraMode_ThirdPerson.generated.h"

/**
 * Standard third person camera.
 *
 * This mode owns the boom: each frame it places the camera at PivotLocation + TargetOffset, pulled
 * back along the pivot's view direction by TargetArmLength, sweeping for geometry so the camera does
 * not clip through the world. Optional position and rotation lag smooth out sudden movement.
 */
UCLASS(BlueprintType, Blueprintable)
class PSOUL_API USoulCameraMode_ThirdPerson : public USoulCameraMode
{
	GENERATED_BODY()

public:
	USoulCameraMode_ThirdPerson();

protected:
	virtual void UpdateView(float DeltaTime) override;
	virtual void OnActivation() override;

	/**
	 * Sweeps a sphere from PivotLocation toward DesiredLocation and reports where the camera can sit
	 * without intersecting level geometry.
	 */
	FVector ResolveCameraPenetration(const FVector& PivotLocation, const FVector& DesiredLocation) const;

protected:
	/** Offset from the pivot, in pivot rotation space: X forward, Y right, Z up. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson")
	FVector TargetOffset = FVector(0.f, 0.f, 60.f);

	/** Distance the camera is pulled back from the offset position. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float TargetArmLength = 400.f;

	/** When true the pivot pitch is clamped to ViewPitchMin/ViewPitchMax before building the boom. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson")
	bool bClampPivotPitch = true;

	/** When true a sweep is performed so geometry between the pivot and the camera pulls the camera in. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Collision")
	bool bDoCollisionTest = true;

	/** Radius of the sphere swept from the pivot to the desired camera position. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Collision", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float CollisionProbeSize = 12.f;

	/** Extra gap kept between the camera and any blocking geometry, so the near plane does not clip. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Collision", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float CollisionPenetrationOffset = 10.f;

	/** Trace channel used for the camera collision sweep. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Collision")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Camera;

	/** When true the camera position trails the ideal position instead of tracking it exactly. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Lag")
	bool bEnableCameraLag = false;

	/** How quickly the lagged camera position catches up. Higher is tighter. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Lag", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float CameraLagSpeed = 10.f;

	/** Upper bound on how far the lagged camera may fall behind the ideal position. 0 means unlimited. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Lag", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float CameraLagMaxDistance = 0.f;

	/** When true the camera rotation trails the ideal rotation instead of tracking it exactly. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Lag")
	bool bEnableCameraRotationLag = false;

	/** How quickly the lagged camera rotation catches up. Higher is tighter. */
	UPROPERTY(EditDefaultsOnly, Category = "ThirdPerson|Lag", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float CameraRotationLagSpeed = 12.f;

private:
	// Ideal camera position produced last frame, used as the lag starting point.
	FVector LastCameraLocation = FVector::ZeroVector;

	// Ideal camera rotation produced last frame, used as the lag starting point.
	FRotator LastCameraRotation = FRotator::ZeroRotator;

	// False until the lag state has been seeded, so the first frame after activation does not
	// interpolate in from stale values.
	bool bLagInitialized = false;
};
