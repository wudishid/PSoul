// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOCKTARGETSYSTEM_API ULockTargetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UBorder* LockIcon;
};
