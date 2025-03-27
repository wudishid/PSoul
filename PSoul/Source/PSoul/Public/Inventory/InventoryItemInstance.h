// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemDefinition.h"
#include "GameFramework/Actor.h"
#include "InventoryItemInstance.generated.h"


class UBoxComponent;

UCLASS()
class PSOUL_API AInventoryItemInstance : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventoryItemInstance();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnBoxCompOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite, Category = "InventoryItemInstance")
	USceneComponent* RootComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite, Category = "InventoryItemInstance")
	UStaticMeshComponent* ItemMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite, Category = "InventoryItemInstance")
	UBoxComponent* BoxComp;
	
	UPROPERTY(EditAnywhere)
	FName ItemName;
	
	FInventoryItemInfo ItemInfo;
public:
};
