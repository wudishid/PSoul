// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/AICharacterBase.h"
#include "AI/SoulAIControllerBase.h"
#include "Data/AICharacterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Util/Util_Common.h"


// Sets default values
AAICharacterBase::AAICharacterBase()
{
}

FRotator AAICharacterBase::GetDesiredRotation() const
{
	if (ASoulAIControllerBase* AIController = Cast<ASoulAIControllerBase>(GetController()))
	{
		if (AActor* AttackTarget = AIController->GetCurrentAttackTarget())
		{
			return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
		}
	}
	
	return Super::GetDesiredRotation();
}

FName AAICharacterBase::GetCharacterName() const
{
	return AICharacterData->AICharacterName;
}

void AAICharacterBase::OnDeath()
{
	Super::OnDeath();

	if (AAIController* AC = GetController<AAIController>())
	{
		GetCharacterMovement()->StopMovementImmediately();
		DetachFromControllerPendingDestroy();
		AC->Destroy();
	}
}

void AAICharacterBase::FinishDeath()
{
	Super::FinishDeath();
	DropItem();
	Destroy();
}

void AAICharacterBase::DropItem_Implementation()
{
	if (!AICharacterData->DropedItemsInfo.IsEmpty())
	{
		int32 DropedItemCount = 0;
		
		for (const auto &ItemInfo : AICharacterData->DropedItemsInfo)
		{
			if (!ItemInfo.DropedItems.IsEmpty())
			{
				for (const auto &Item : ItemInfo.DropedItems)
				{
					if (FMath::RandRange(0.f, 1.f) <= ItemInfo.DropProbability)
					{
						Util_Common::SpawnInventoryItemInstance(this, Item);
						
						if (++DropedItemCount >= AICharacterData->MaxDropItemAmount) return;
					}
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(AICharacterData);
	ShowHealthBar();
}



