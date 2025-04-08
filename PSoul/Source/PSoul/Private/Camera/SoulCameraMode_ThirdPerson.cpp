// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SoulCameraMode_ThirdPerson.h"

#include "Camera/SoulCameraComponent.h"

void USoulCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	View.Location = GetCameraComponent()->GetComponentLocation();
	View.Rotation = GetCameraComponent()->GetComponentRotation();
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}
