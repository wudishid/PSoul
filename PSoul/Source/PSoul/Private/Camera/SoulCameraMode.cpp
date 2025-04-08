// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SoulCameraMode.h"

#include "Camera/SoulCameraComponent.h"

USoulCameraMode::USoulCameraMode()
{
	
}

USoulCameraComponent* USoulCameraMode::GetCameraComponent() const
{
	return CastChecked<USoulCameraComponent>(GetOuter());
}

UWorld* USoulCameraMode::GetWorld() const
{
	return HasAnyFlags(RF_DefaultSubObject) ? nullptr : GetOuter()->GetWorld();
}

AActor* USoulCameraMode::GetTargetActor() const
{
	return GetCameraComponent()->GetTargetActor();
}

void USoulCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
}

void USoulCameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

FVector USoulCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if(const APawn* Pawn = Cast<APawn>(TargetActor))
	{
		return Pawn->GetPawnViewLocation();
	}
	
	return TargetActor->GetActorLocation();
}

FRotator USoulCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if(const APawn* Pawn = Cast<APawn>(TargetActor))
	{
		return Pawn->GetViewRotation();
	}
	
	return TargetActor->GetActorRotation();
}
