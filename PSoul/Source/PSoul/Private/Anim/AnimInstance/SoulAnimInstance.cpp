// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim/AnimInstance/SoulAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AnimGraphRuntime/Public/KismetAnimationLibrary.h"

void USoulAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(APawn* OwnerPawn = TryGetPawnOwner())
	{
		Speed = UKismetMathLibrary::VSizeXY(OwnerPawn->GetVelocity());
		Speed = UKismetMathLibrary::MapRangeClamped(Speed, 0, 600, 0, 1);
		Direction = UKismetAnimationLibrary::CalculateDirection(OwnerPawn->GetVelocity(), OwnerPawn->GetActorRotation());
		Direction =  UKismetMathLibrary::MapRangeClamped(Direction, -180, 180, -1, 1);

		if(UCharacterMovementComponent* MoveComp = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>())
		{
			IsFalling = MoveComp->IsFalling();
		}
	}
}
