// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/StateBar.h"

#include "Components/CharacterAttributeComponent.h"
#include "Components/ProgressBar.h"

void UStateBar::NativeConstruct()
{
	Super::NativeConstruct();
	StateBar->SetFillColorAndOpacity(BarFillColor);
}

void UStateBar::Init(APawn* OwnerPawn)
{
	if(ensureMsgf(OwnerPawn, TEXT("UStateBar::Init InOwnerPawn is nullptr")))
	{
		AttributeComponent = OwnerPawn->FindComponentByClass<UCharacterAttributeComponent>();
		if(ensureMsgf(AttributeComponent, TEXT("Pawn Has not AttributeComponent!")))
		{
			if(!AttributeComponent->OnAttributeChanged.IsAlreadyBound(this, &ThisClass::HandleAttributeChanged))
			{
				AttributeComponent->OnAttributeChanged.AddDynamic(this, &ThisClass::HandleAttributeChanged);
			}
			
			UpdateBar();
		}
	}
}

void UStateBar::Init(APawn* OwnerPawn, FGameplayAttribute InAttribute, FGameplayAttribute InMaxAttribute)
{
	if(ensureMsgf(OwnerPawn, TEXT("UStateBar::Init InOwnerPawn is nullptr")))
	{
		AttributeComponent = OwnerPawn->FindComponentByClass<UCharacterAttributeComponent>();
		if(ensureMsgf(AttributeComponent, TEXT("Pawn Has not AttributeComponent!")))
		{
			if(!AttributeComponent->OnAttributeChanged.IsAlreadyBound(this, &ThisClass::HandleAttributeChanged))
			{
				AttributeComponent->OnAttributeChanged.AddDynamic(this, &ThisClass::HandleAttributeChanged);
			}
			
			Attribute = InAttribute;
			MaxAttribute = InMaxAttribute;

			UpdateBar();
		}
	}
}

void UStateBar::HandleAttributeChanged(FGameplayAttribute InAttribute, float InCurrentValue, float InOldValue)
{
	if (InAttribute == Attribute || InAttribute == MaxAttribute)
	{
		UpdateBar();
	}
}

void UStateBar::UpdateBar()
{
	float CurValue = AttributeComponent->GetAttributeValue(Attribute);
	float MaxValue = AttributeComponent->GetAttributeValue(MaxAttribute);
	StateBar->SetPercent(CurValue / MaxValue);
}
