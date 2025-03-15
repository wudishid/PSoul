// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UI/SoulUserWidget.h"
#include "StateBar.generated.h"


class UCharacterAttributeComponent;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PSOUL_API UStateBar : public USoulUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Init(APawn* OwnerPawn);
	void Init(APawn* OwnerPawn, FGameplayAttribute InAttribute, FGameplayAttribute InMaxAttribute);
	
	UFUNCTION()
	void HandleAttributeChanged(FGameplayAttribute InAttribute, float InCurrentValue, float InOldValue);

	void UpdateBar();
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StateBar;

	UPROPERTY(EditAnywhere, Category = "StateBar")
	FLinearColor BarFillColor = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, Category = "StateBar")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, Category = "StateBar")
	FGameplayAttribute MaxAttribute;
	
	UPROPERTY()
	UCharacterAttributeComponent* AttributeComponent;
};
