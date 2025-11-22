// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoulAnimInstance.generated.h"

class USoulAbilitySystemComponent;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PSOUL_API USoulAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	//0-1
	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	float Direction;

	//0-1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintreadOnly)
	bool IsFalling;
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
};
