// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/StateBar.h"

#include "Components/CharacterAttributeComponent.h"
#include "Components/ProgressBar.h"

void UStateBar::NativeConstruct()
{
	Super::NativeConstruct();
	StateBar->SetFillColorAndOpacity(BarFillColor);
}

void UStateBar::Init(APawn* OwnerPawn, FName InAttributeName, FName InMaxAttributeName)
{
	if(ensureMsgf(OwnerPawn, TEXT("UStateBar::Init InOwnerPawn is nullptr")))
	{
		if(UCharacterAttributeComponent* AttributeComponent = OwnerPawn->FindComponentByClass<UCharacterAttributeComponent>())
		{
			AttributeComponent->OnAttributeChanged.AddDynamic(this, &ThisClass::HandleAttributeChanged);
			AttributeName = InAttributeName;
			MaxAttributeName = InMaxAttributeName;
		}
	}
}

void UStateBar::HandleAttributeChanged(FGameplayAttribute Attribute, float InCurrentValue, float InOldValue)
{
	if(Attribute.GetName() == AttributeName)
	{
		CurrentValue = InCurrentValue;
		StateBar->SetPercent(CurrentValue / MaxValue);
	}
	else if(Attribute.GetName() == MaxAttributeName)
	{
		MaxValue = InCurrentValue;
		StateBar->SetPercent(CurrentValue / MaxValue);
	}
	
}
