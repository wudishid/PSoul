// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Attribute/SoulAttributeSet.h"



USoulAttributeSet::USoulAttributeSet()
{
}

float USoulAttributeSet::GetAttributeValue(FGameplayAttribute& InAttribute) const 
{
	TArray<FGameplayAttribute> Attributes;
	GetAttributesFromSetClass(GetClass(),  Attributes);
	for(const FGameplayAttribute& Attribute : Attributes)
	{
		if(Attribute == InAttribute)
		{
			return Attribute.GetNumericValue(this);
		}
	}
	
	return 0.f;
}
