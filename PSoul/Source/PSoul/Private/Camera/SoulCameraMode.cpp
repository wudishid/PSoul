// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SoulCameraMode.h"

#include "Camera/SoulCameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"

FSoulCameraModeView::FSoulCameraModeView()
	: Location(FVector::ZeroVector)
	, Rotation(FRotator::ZeroRotator)
	, ControlRotation(FRotator::ZeroRotator)
	, FieldOfView(80.f)
{
}

void FSoulCameraModeView::Blend(const FSoulCameraModeView& Other, float OtherWeight)
{
	const float Weight = FMath::Clamp(OtherWeight, 0.f, 1.f);
	if (Weight <= 0.f)
	{
		return;
	}

	if (Weight >= 1.f)
	{
		*this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, Weight);
	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, Weight);

	// Take the short way around, otherwise a 350 -> 10 degree yaw change spins almost all the way back.
	Rotation = Rotation + ((Other.Rotation - Rotation).GetNormalized() * Weight);
	ControlRotation = ControlRotation + ((Other.ControlRotation - ControlRotation).GetNormalized() * Weight);
}

USoulCameraMode::USoulCameraMode()
{
}

USoulCameraComponent* USoulCameraMode::GetCameraComponent() const
{
	return CastChecked<USoulCameraComponent>(GetOuter());
}

UWorld* USoulCameraMode::GetWorld() const
{
	// Modes are instanced and owned by the camera component, so defer to it for the world.
	// The class default object has no world, and asking it for one would assert.
	if (HasAnyFlags(RF_ClassDefaultObject | RF_DefaultSubObject))
	{
		return nullptr;
	}

	const UObject* Outer = GetOuter();
	return Outer ? Outer->GetWorld() : nullptr;
}

AActor* USoulCameraMode::GetTargetActor() const
{
	return GetCameraComponent()->GetTargetActor();
}

void USoulCameraMode::OnActivation()
{
	// Start fully faded out so the camera eases in from the previous view; UpdateBlending ramps this to 1.
	BlendWeight = (BlendTime > 0.f) ? 0.f : 1.f;
}

void USoulCameraMode::OnDeactivation()
{
}

void USoulCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void USoulCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f && BlendWeight < 1.f)
	{
		BlendWeight = FMath::Clamp(BlendWeight + (DeltaTime / BlendTime), 0.f, 1.f);
	}
	else
	{
		BlendWeight = 1.f;
	}
}

void USoulCameraMode::UpdateView(float DeltaTime)
{
	const FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = GetFieldOfView();
}

FVector USoulCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// Characters crouch by shrinking their capsule. Keep the pivot at a consistent eye height so the
		// camera does not drop when the capsule shrinks.
		if (const ACharacter* TargetCharacter = Cast<ACharacter>(TargetPawn))
		{
			const UCapsuleComponent* CapsuleComp = TargetCharacter->GetCapsuleComponent();
			const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			const UCapsuleComponent* CapsuleCompCDO = TargetCharacterCDO ? TargetCharacterCDO->GetCapsuleComponent() : nullptr;

			if (CapsuleComp && CapsuleCompCDO)
			{
				const float DefaultHalfHeight = CapsuleCompCDO->GetUnscaledCapsuleHalfHeight();
				const float ActualHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
				const float HeightAdjustment = (DefaultHalfHeight - ActualHalfHeight) + TargetCharacterCDO->BaseEyeHeight;

				return TargetCharacter->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
			}
		}

		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator USoulCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// For a controlled pawn this is the controller's control rotation, which is what we want the
		// camera to follow.
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}
