// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/SoulAbilitySet.h"
#include "GAS/SoulGameplayAbilityTargetTypes.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "GAS/GameplayAbility/GameplayAbility_CombAttack.h"
#include "GAS/GameplayAbility/SoulGameplayAbility.h"
#include "PSoul/SoulGameplayTags.h"


// Sets default values for this component's properties
USoulAbilitySystemComponent::USoulAbilitySystemComponent()
{
}

void USoulAbilitySystemComponent::ConsumeInputBuffer()
{
	if (LastInputBufferAbilityTags.IsValid())
	{
		TryActivateAbilitiesByTag(LastInputBufferAbilityTags);
		LastInputBufferAbilityTags.Reset();
	}
}

void USoulAbilitySystemComponent::AddGameplayTags(FGameplayTagContainer Tags)
{
	AddLooseGameplayTags(Tags);
}

void USoulAbilitySystemComponent::RemoveGameplayTags(FGameplayTagContainer Tags)
{
	RemoveLooseGameplayTags(Tags);
}

// Called when the game starts
void USoulAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USoulAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (USoulGameplayAbility* Ability = Cast<USoulGameplayAbility>(AbilitySpec.Ability))
	{
		if (Ability->GetActivationPolicy() == ESoulAbilityActivationPolicy::OnSpawn)
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void USoulAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	bool bNewAvatatActor = AbilityActorInfo.Get()->AvatarActor != InAvatarActor;

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bNewAvatatActor)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(this);
		}
	}
}

void USoulAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void USoulAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void USoulAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();
	
	if (HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("InputBuffer"))))
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT("has buffer"));
	}
	
	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const USoulGameplayAbility* SoulAbilityCDO = Cast<USoulGameplayAbility>(AbilitySpec->Ability);
				if (SoulAbilityCDO && SoulAbilityCDO->GetActivationPolicy() == ESoulAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					if (AbilitySpec->Ability.GetClass()->IsChildOf(UGameplayAbility_CombAttack::StaticClass()))
					{
						if (UGameplayAbility_CombAttack* Ability_CombAttack = Cast<UGameplayAbility_CombAttack>(AbilitySpec->GetPrimaryInstance()))
						{
							if (Ability_CombAttack->bComb)
							{
								Ability_CombAttack->bComb = false;

								int32 CombIndex = Ability_CombAttack->CurrentCombIndex + 1;
								FGameplayAbilityTargetData_AttackInfo* AttackInfo = new FGameplayAbilityTargetData_AttackInfo();
								AttackInfo->CombAttackIndex = CombIndex;
								if (APawn* OwnerPawn = Cast<APawn>(GetAvatarActor()))
								{
									AttackInfo->InputVector = OwnerPawn->GetLastMovementInputVector();
								}
								FGameplayEventData EventData;
								EventData.TargetData.Add(AttackInfo);
								InternalTryActivateAbility(AbilitySpec->Handle, FPredictionKey(), nullptr, nullptr, &EventData);
							}
						}
						continue;
					}
					
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					//输入缓冲
					if (HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("InputBuffer"))))

					{
						if (UGameplayAbility* AnimatingAbility = GetAnimatingAbility())
						{
							if (AnimatingAbility->IsBlockingOtherAbilities())
							{
								LastInputBufferAbilityTags = AbilitySpec->Ability->AbilityTags;
							}
						}
					}


					if (AbilitySpec->Ability->AbilityTags.HasTag(SoulGameplayTags::Ability_Action_Attack) ||
						AbilitySpec->Ability->AbilityTags.HasTag(SoulGameplayTags::Ability_Action_Roll)
						)
					{
						FGameplayAbilityTargetData_AttackInfo* AttackInfo = new FGameplayAbilityTargetData_AttackInfo();
						if (APawn* OwnerPawn = Cast<APawn>(GetAvatarActor()))
						{
							AttackInfo->InputVector = OwnerPawn->GetLastMovementInputVector();
						}
						FGameplayEventData EventData;
						EventData.TargetData.Add(AttackInfo);
						InternalTryActivateAbility(AbilitySpec->Handle, FPredictionKey(), nullptr, nullptr, &EventData);
						continue;
					}
					
					const USoulGameplayAbility* SoulAbilityCDO = Cast<USoulGameplayAbility>(AbilitySpec->Ability);
					if (SoulAbilityCDO && SoulAbilityCDO->GetActivationPolicy() == ESoulAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void USoulAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void USoulAbilitySystemComponent::Server_AddAttributePoint_Implementation(FGameplayAttribute InAttribute)
{
	TObjectPtr<const USoulPlayerSet> TempSet = GetSet<USoulPlayerSet>();
	if (TObjectPtr<USoulPlayerSet> PlayerSet = ConstCast(TempSet))
	{
		if (PlayerSet->AddAttributePoint(InAttribute))
		{
		}
	}
}



