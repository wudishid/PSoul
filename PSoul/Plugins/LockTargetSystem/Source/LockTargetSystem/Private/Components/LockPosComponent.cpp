// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LockPosComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/LockTargetWidget.h"


// Sets default values for this component's properties
ULockPosComponent::ULockPosComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void ULockPosComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor*OwnerActor=GetOwner())
	{
		WidgetComponent=Cast<UWidgetComponent>(OwnerActor->AddComponentByClass(UWidgetComponent::StaticClass(),false,FTransform(),false));

		if(WidgetComponent&&LockTargetWidgetClass)
		{
			WidgetComponent->SetWidgetClass(LockTargetWidgetClass);
			WidgetComponent->SetDrawAtDesiredSize(true);
			WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			WidgetComponent->SetVisibility(false);
		}
	}
	
}

void ULockPosComponent::OnUnregister()
{
	Super::OnUnregister();

	if(WidgetComponent)
	{
		WidgetComponent->SetVisibility(false);
		WidgetComponent->DestroyComponent();
	}
}

void ULockPosComponent::SetShowLockPosIcon(bool InShow)
{
	if(WidgetComponent)
	{
		WidgetComponent->SetVisibility(InShow);
	}
}

