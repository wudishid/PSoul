// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "GameplayAbility_CombAttack.generated.h"

USTRUCT(BlueprintType)
struct FCombAttackInfo
{
	GENERATED_BODY()

	FCombAttackInfo() : Montage(nullptr), CombIndex(0)
	{
		
	}
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	int32 CombIndex;
};


UCLASS()
class PSOUL_API UGameplayAbility_CombAttack : public USoulGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void PlayMontageAndWaitForEvent();
	
	UFUNCTION()
	void HandleOpenCombWindow(FGameplayEventData Payload);

	UFUNCTION()
	void HandleCloseCombWindow(FGameplayEventData Payload);

	UFUNCTION()
	void HandleMontageEnded();

	UFUNCTION()
	void HandleAttackCancelled();
	
public:
	UGameplayAbility_CombAttack();

	UFUNCTION(BlueprintPure, Category = "CombAttack")
	UAnimMontage* GetCurrentCombMontage();
	
	UPROPERTY(Replicated)
	mutable  int32 CurrentCombIndex;

	UPROPERTY(Replicated)
	mutable  bool bComb;
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<FCombAttackInfo> CombAttackInfos;
};
