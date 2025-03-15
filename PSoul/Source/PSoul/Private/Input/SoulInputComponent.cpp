// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SoulInputComponent.h"



USoulInputComponent::USoulInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void USoulInputComponent::AddInputMappings(const USoulInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void USoulInputComponent::RemoveInputMappings(const USoulInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void USoulInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
