// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Player/Attribute/AttributeRow.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Kismet/KismetTextLibrary.h"

void UAttributeRow::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(MaxAttribute.IsValid())
	{
		HB_AttributeMaxValue->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HB_AttributeMaxValue->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAttributeRow::NativeConstruct()
{
	Super::NativeConstruct();

	if (USoulAbilitySystemComponent* ASC = GetOwningPlayerPawn()->FindComponentByClass<USoulAbilitySystemComponent>())
	{
		TextBlock_AttributeValue->SetText(UKismetTextLibrary::Conv_IntToText(ASC->GetNumericAttribute(Attribute)));
		
		if (!ASC->GetGameplayAttributeValueChangeDelegate(Attribute).IsBoundToObject(this))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::HandleAttributeChanged);
		}

		if(MaxAttribute.IsValid())
		{
			TextBlock_AttributeMaxValue->SetText(UKismetTextLibrary::Conv_IntToText(ASC->GetNumericAttribute(MaxAttribute)));
			
			if (!ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).IsBoundToObject(this))
			{
				ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &ThisClass::HandleMaxAttributeChanged);
			}
		}
	}
}

void UAttributeRow::HandleAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	TextBlock_AttributeValue->SetText(UKismetTextLibrary::Conv_IntToText(ChangeData.NewValue));
}

void UAttributeRow::HandleMaxAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	TextBlock_AttributeMaxValue->SetText(UKismetTextLibrary::Conv_IntToText(ChangeData.NewValue));
}
