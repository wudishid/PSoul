// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SoulCameraMode_LockTarget.h"
#include "Kismet/KismetMathLibrary.h"

USoulCameraMode_LockTarget::USoulCameraMode_LockTarget()
{
	// The character body should face whatever the camera is locked onto.
	bApplyControlRotationToOwner = true;
}

void USoulCameraMode_LockTarget::OnActivation()
{
	Super::OnActivation();

	// Remember the relaxed boom length so it can be handed back on deactivation.
	CachedFreeArmLength = TargetArmLength;
	LockedArmLengthAlpha = 0.f;

	// Seed from the current pivot rotation so the first frame does not swing in from the origin.
	BlendedPivotRotation = Super::GetPivotRotation();
}

void USoulCameraMode_LockTarget::OnDeactivation()
{
	if (CachedFreeArmLength >= 0.f)
	{
		TargetArmLength = CachedFreeArmLength;
		CachedFreeArmLength = -1.f;
	}

	LockedArmLengthAlpha = 0.f;

	Super::OnDeactivation();
}

AActor* USoulCameraMode_LockTarget::GetLockTarget() const
{
	const AActor* TargetActor = GetTargetActor();
	if (!TargetActor)
	{
		return nullptr;
	}

	return nullptr;
}

FRotator USoulCameraMode_LockTarget::GetPivotRotation() const
{
	// The inherited third person maths builds its offset, arm and collision from this rotation.
	return BlendedPivotRotation;
}

void USoulCameraMode_LockTarget::UpdateView(float DeltaTime)
{
	// Pitch and roll come straight from the controller, so the player always keeps vertical aim. Yaw is
	// driven from our own previous rotation instead, so this frame's look input cannot fight the lock.
	const FRotator CurrentControlRotation = Super::GetPivotRotation();

	FRotator PivotRotation = BlendedPivotRotation;
	PivotRotation.Pitch = CurrentControlRotation.Pitch;
	PivotRotation.Roll = CurrentControlRotation.Roll;

	AActor* LockTarget = GetLockTarget();
	if (LockTarget)
	{
		const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetPivotLocation(), LockTarget->GetActorLocation());

		// Ease onto the target rather than snapping to it, so engaging and disengaging lock-on are both
		// continuous and the target settles into the centre of the screen.
		FRotator Desired = PivotRotation;
		Desired.Yaw = LookAt.Yaw;
		Desired.Roll = 0.f;
		if (bLockPitch)
		{
			Desired.Pitch = FMath::ClampAngle(LookAt.Pitch, ViewPitchMin, ViewPitchMax);
		}

		PivotRotation = FMath::RInterpTo(PivotRotation, Desired, DeltaTime, LockRotationInterpSpeed);
	}

	BlendedPivotRotation = PivotRotation;

	// Tighten the boom while a target is held, and relax it back out when the target is lost.
	if (CachedFreeArmLength >= 0.f)
	{
		const float TargetAlpha = LockTarget ? 1.f : 0.f;
		LockedArmLengthAlpha = FMath::FInterpTo(LockedArmLengthAlpha, TargetAlpha, DeltaTime, ArmLengthInterpSpeed);
		TargetArmLength = FMath::Lerp(CachedFreeArmLength, LockedArmLength, LockedArmLengthAlpha);
	}

	// Runs the inherited boom, collision and lag maths, which reads GetPivotRotation().
	Super::UpdateView(DeltaTime);
}
