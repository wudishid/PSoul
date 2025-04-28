// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim/AnimInstance/SoulAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PSoul/SoulGameplayTags.h"
#include "Runtime/AnimGraphRuntime/Public/KismetAnimationLibrary.h"

void USoulAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CharacterMovementComponent = TryGetPawnOwner()->FindComponentByClass<UCharacterMovementComponent>();
	ASC = TryGetPawnOwner()->FindComponentByClass<USoulAbilitySystemComponent>();
}

void USoulAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(APawn* OwnerPawn = TryGetPawnOwner())
	{
		Speed = UKismetMathLibrary::VSizeXY(OwnerPawn->GetVelocity());
		Speed = UKismetMathLibrary::MapRangeClamped(Speed, 0, 600, 0, 1);
		Direction = UKismetAnimationLibrary::CalculateDirection(OwnerPawn->GetVelocity(), OwnerPawn->GetActorRotation());
		Direction =  UKismetMathLibrary::MapRangeClamped(Direction, -180, 180, -1, 1);
	}

	if(CharacterMovementComponent)
	{
		IsFalling = CharacterMovementComponent->IsFalling();
	}

	if(ASC)
	{
		IsBlocking = ASC->HasMatchingGameplayTag(SoulGameplayTags::Status_Block);
	}
}
