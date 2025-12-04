// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Player/Attribute/AttributePoint.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Kismet/KismetTextLibrary.h"

void UAttributePoint::Init()
{
	ASC = GetOwningPlayerPawn()->FindComponentByClass<USoulAbilitySystemComponent>();
	check(ASC);
	TextBlock_AttributePointValue->SetText(UKismetTextLibrary::Conv_FloatToText(ASC->GetNumericAttribute(Attribute), ToPositiveInfinity));
	if (!ASC->GetGameplayAttributeValueChangeDelegate(Attribute).IsBoundToObject(this))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::HandleAttributeChanged);
	}

	Btn_AddPoint->OnClicked.RemoveAll(this);
	Btn_AddPoint->OnClicked.AddDynamic(this, &ThisClass::HandleBtn_AddPointClicked);
}

void UAttributePoint::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAttributePoint::HandleAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	TextBlock_AttributePointValue->SetText(UKismetTextLibrary::Conv_FloatToText(ChangeData.NewValue, ToPositiveInfinity));
}

void UAttributePoint::HandleBtn_AddPointClicked()
{
	if (ASC)
	{
		ASC->Server_AddAttributePoint(Attribute);
	}
}
