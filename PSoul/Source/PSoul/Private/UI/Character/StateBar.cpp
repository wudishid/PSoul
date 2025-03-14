// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/StateBar.h"

#include "Components/CharacterAttributeComponent.h"
#include "Components/ProgressBar.h"

void UStateBar::NativeConstruct()
{
	Super::NativeConstruct();

	if(APawn* OwnerPawn = GetOwningPlayerPawn())
	{
		if(UCharacterAttributeComponent* AttributeComponent = OwnerPawn->FindComponentByClass<UCharacterAttributeComponent>())
		{
			AttributeComponent->OnAttributeChanged.AddDynamic(this, &ThisClass::HandleAttributeChanged);
		}
	}

	StateBar->SetFillColorAndOpacity(BarFillColor);
}

void UStateBar::HandleAttributeChanged(FGameplayAttribute Attribute, float InCurrentValue, float InOldValue)
{
	if(Attribute.GetName() == AttributeName)
	{
		CurrentValue = InCurrentValue;
	}
	else if(Attribute.GetName() == MaxAttributeName)
	{
		MaxValue = InCurrentValue;
	}

	StateBar->SetPercent(CurrentValue / MaxValue);
}
