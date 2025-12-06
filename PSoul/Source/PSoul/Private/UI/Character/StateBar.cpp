// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Character/StateBar.h"
#include "../../../../../Plugins/AdvanceProgressBar/Source/AdvanceProgressBar/Public/UAdvanceBar.h"
#include "Components/CharacterAttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UStateBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	Bar->SetBarWidth(BarWidth);
	Bar->SetBarColor(FrontBarColor, BackBarColor);
}

void UStateBar::NativeConstruct()
{
	Super::NativeConstruct();
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
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Cur%f : Max%f"), CurValue, MaxValue), true, true, FLinearColor::Yellow, 5.f);
	Bar->SetBarWidth(MaxValue);
	Bar->UpdateBar(CurValue / MaxValue, EAdvanceBarUpdateStyle::Lerp);
}
