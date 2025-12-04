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

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
public:
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

	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	float BarWidth = 150.f;
	
	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	FLinearColor FrontBarColor=FLinearColor::Red;;

	UPROPERTY(EditAnywhere,Category="AdvanceBar")
	FLinearColor BackBarColor=FLinearColor::Yellow;
	
	UPROPERTY()
	UCharacterAttributeComponent* AttributeComponent;
};
