// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UI/SoulUserWidget.h"
#include "StateBar.generated.h"


class UAdvanceBar;
class UCharacterAttributeComponent;

/**
 * 
 */
UCLASS()
class PSOUL_API UStateBar : public USoulUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void Init(APawn* OwnerPawn);
	void Init(APawn* OwnerPawn, FGameplayAttribute InAttribute, FGameplayAttribute InMaxAttribute);
	
	UFUNCTION()
	void HandleAttributeChanged(FGameplayAttribute InAttribute, float InCurrentValue, float InOldValue);

	void UpdateBar();
protected:
	UPROPERTY(meta = (BindWidget))
	UAdvanceBar* Bar;
	
	UPROPERTY(EditAnywhere, Category = "StateBar")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, Category = "StateBar")
	FGameplayAttribute MaxAttribute;

	UPROPERTY(EditAnywhere,Category="StateBar")
	FLinearColor FrontBarColor=FLinearColor::Red;;

	UPROPERTY(EditAnywhere,Category="StateBar")
	FLinearColor BackBarColor=FLinearColor::Yellow;

	
	UPROPERTY()
	UCharacterAttributeComponent* AttributeComponent;
};
