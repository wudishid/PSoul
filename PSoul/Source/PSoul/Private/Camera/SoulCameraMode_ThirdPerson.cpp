// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SoulCameraMode_ThirdPerson.h"

#include "Camera/SoulCameraComponent.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/World.h"

USoulCameraMode_ThirdPerson::USoulCameraMode_ThirdPerson()
{
}

void USoulCameraMode_ThirdPerson::OnActivation()
{
	Super::OnActivation();

	// Seed the lag state on the first update so we do not interpolate in from stale values.
	bLagInitialized = false;
}

void USoulCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	const FVector PivotLocation = GetPivotLocation();

	FRotator PivotRotation = GetPivotRotation();
	if (bClampPivotPitch)
	{
		PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	}

	// Ideal camera position: offset from the pivot, then pulled back along the view direction.
	const FVector OffsetLocation = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	FVector DesiredLocation = OffsetLocation - (PivotRotation.Vector() * TargetArmLength);

	if (bDoCollisionTest)
	{
		DesiredLocation = ResolveCameraPenetration(OffsetLocation, DesiredLocation);
	}

	// Position lag.
	if (bEnableCameraLag && CameraLagSpeed > 0.f)
	{
		if (bLagInitialized)
		{
			const FVector LaggedLocation = FMath::VInterpTo(LastCameraLocation, DesiredLocation, DeltaTime, CameraLagSpeed);
			const FVector LagOffset = LaggedLocation - DesiredLocation;
			const bool bExceedsMaxLag = (CameraLagMaxDistance > 0.f) && (LagOffset.SizeSquared() > FMath::Square(CameraLagMaxDistance));

			// Clamp how far the lagged position may trail the ideal one, then adopt it.
			DesiredLocation = bExceedsMaxLag
				? DesiredLocation + (LagOffset.GetSafeNormal() * CameraLagMaxDistance)
				: LaggedLocation;
		}
	}

	LastCameraLocation = DesiredLocation;

	// Rotation lag.
	FRotator DesiredRotation = PivotRotation;
	if (bEnableCameraRotationLag && CameraRotationLagSpeed > 0.f && bLagInitialized)
	{
		DesiredRotation = FMath::RInterpTo(LastCameraRotation, DesiredRotation, DeltaTime, CameraRotationLagSpeed);
	}

	LastCameraRotation = DesiredRotation;
	bLagInitialized = true;

	View.Location = DesiredLocation;
	View.Rotation = DesiredRotation;
	View.ControlRotation = PivotRotation;
	View.FieldOfView = GetFieldOfView();
}

FVector USoulCameraMode_ThirdPerson::ResolveCameraPenetration(const FVector& PivotLocation, const FVector& DesiredLocation) const
{
	const UWorld* World = GetWorld();
	const AActor* TargetActor = GetTargetActor();
	if (!World || !TargetActor)
	{
		return DesiredLocation;
	}

	const FVector TraceDelta = DesiredLocation - PivotLocation;
	const float DesiredDistance = TraceDelta.Size();

	// Degenerate boom: nothing meaningful to sweep along.
	constexpr float MinTraceDistance = 1.e-4f;
	if (DesiredDistance <= MinTraceDistance)
	{
		return DesiredLocation;
	}

	const FVector TraceDirection = TraceDelta / DesiredDistance;

	// Ignoring the camera's own owner keeps the character from blocking its own camera.
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SoulCameraPenetration), /*bInTraceComplex=*/false, TargetActor);
	QueryParams.bReturnPhysicalMaterial = false;

	FHitResult Hit;
	const bool bHit = World->SweepSingleByChannel(
		Hit,
		PivotLocation,
		DesiredLocation,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(FMath::Max(CollisionProbeSize, 0.f)),
		QueryParams);

	if (!bHit)
	{
		return DesiredLocation;
	}

	// Hit.Time is the fraction along the sweep at which the probe first touched, so scale it back to a
	// distance and keep a small gap so the camera's near plane does not clip the obstacle.
	const float SafeDistance = FMath::Max((Hit.Time * DesiredDistance) - CollisionPenetrationOffset, 0.f);
	return PivotLocation + (TraceDirection * SafeDistance);
}
