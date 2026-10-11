// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/SoulCameraMode_ThirdPerson.h"
#include "SoulCameraMode_LockTarget.generated.h"

/**
 * Third person camera that keeps a locked-on target centred.
 *
 * The boom, collision and lag behaviour is inherited from USoulCameraMode_ThirdPerson; this mode only
 * overrides the pivot rotation. It eases the pivot onto the target rather than snapping, so both
 * entering and leaving lock-on stay continuous, and it hands the resulting rotation back to the
 * controller so the character body faces what the camera is locked onto.
 *
 * The target itself is supplied by the ULockTargetComponent on the camera's owner, which remains the
 * authority on which actor is locked and replicates that state.
 */
UCLASS(BlueprintType, Blueprintable)
class PSOUL_API USoulCameraMode_LockTarget : public USoulCameraMode_ThirdPerson
{
	GENERATED_BODY()

public:
	USoulCameraMode_LockTarget();

	virtual void OnActivation() override;
	virtual void OnDeactivation() override;

	/** The actor currently locked on to, or null when there is none. */
	AActor* GetLockTarget() const;

protected:
	virtual void UpdateView(float DeltaTime) override;
	virtual FRotator GetPivotRotation() const override;

protected:
	// How quickly the view swings onto the locked target. Higher is snappier.
	UPROPERTY(EditDefaultsOnly, Category = "LockTarget", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float LockRotationInterpSpeed = 8.f;

	// Boom length used while a target is held; usually shorter than the free camera for a tighter framing.
	UPROPERTY(EditDefaultsOnly, Category = "LockTarget", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float LockedArmLength = 300.f;

	// How quickly the boom tightens onto LockedArmLength and relaxes back out again.
	UPROPERTY(EditDefaultsOnly, Category = "LockTarget", meta = (UIMin = "0.0", ClampMin = "0.0"))
	float ArmLengthInterpSpeed = 6.f;

	// When true the pitch tracks the target as well. Off by default so the player keeps vertical aim.
	UPROPERTY(EditDefaultsOnly, Category = "LockTarget")
	bool bLockPitch = false;

private:
	// Pivot rotation produced this frame. GetPivotRotation() feeds it to the inherited boom maths.
	FRotator BlendedPivotRotation = FRotator::ZeroRotator;

	// 0 while fully relaxed, 1 once the boom has fully tightened onto LockedArmLength.
	float LockedArmLengthAlpha = 0.f;

	// Boom length to restore once lock-on ends. Negative means nothing has been cached yet.
	float CachedFreeArmLength = -1.f;
};
