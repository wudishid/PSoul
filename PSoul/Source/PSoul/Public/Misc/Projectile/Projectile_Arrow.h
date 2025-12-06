// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Projectile_Arrow.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class PSOUL_API AProjectile_Arrow : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile_Arrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arrow")
	TObjectPtr<UBoxComponent> CheckBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arrow")
	TObjectPtr<UStaticMeshComponent> ArrowMesh;
};
