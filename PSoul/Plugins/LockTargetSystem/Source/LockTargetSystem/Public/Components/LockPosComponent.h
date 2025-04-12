// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LockPosComponent.generated.h"


class ULockTargetWidget;

DECLARE_MULTICAST_DELEGATE(FOnLockEnd)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOCKTARGETSYSTEM_API ULockPosComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	ULockPosComponent();
	FOnLockEnd OnLockEnd;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnUnregister() override;
public:
	UFUNCTION(BlueprintCallable,Category="LockPos")
	void SetShowLockPosIcon(bool InShow);
private:
	TObjectPtr<class UWidgetComponent> WidgetComponent;

	UPROPERTY(EditDefaultsOnly,Category="LockWidgetClass",meta=(AllowPrivateAccess="True"))
	TSubclassOf<ULockTargetWidget> LockTargetWidgetClass;
};
