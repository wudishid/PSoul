// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SoulAbilitySystemComponent.h"

#include "GAS/SoulAbilitySet.h"


// Sets default values for this component's properties
USoulAbilitySystemComponent::USoulAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USoulAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USoulAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	
	if(AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(this);
	}
	
}


