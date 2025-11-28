// Fill out your copyright notice in the Description page of Project Settings.
#include "Anim/AnimInstance/SoulAnimInstance.h"
#include "Components/SoulCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AnimGraphRuntime/Public/KismetAnimationLibrary.h"

void USoulAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CharacterMovementComponent = TryGetPawnOwner()->FindComponentByClass<USoulCharacterMovementComponent>();
}

void USoulAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(APawn* OwnerPawn = TryGetPawnOwner())
	{
		if (CharacterMovementComponent)
		{
			float TempSpeed = UKismetMathLibrary::VSizeXY(OwnerPawn->GetVelocity());
			Speed = UKismetMathLibrary::MapRangeClamped(TempSpeed, 0, CharacterMovementComponent->DefaultMaxSpeed, 0, 1);
		}
		Direction = UKismetAnimationLibrary::CalculateDirection(OwnerPawn->GetVelocity(), OwnerPawn->GetActorRotation());
		Direction =  UKismetMathLibrary::MapRangeClamped(Direction, -180, 180, -1, 1);
	}

	if(CharacterMovementComponent)
	{
		IsFalling = CharacterMovementComponent->IsFalling();
	}
	
}
