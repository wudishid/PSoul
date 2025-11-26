// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/GameplayEffect/SoulGameplayEffect_Damage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DamageCheckComponent.generated.h"

struct FDamageInfo;
class UGameplayEffect;
struct FGameplayTag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PSOUL_API UDamageCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageCheckComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	void SetCheckByMesh(UPrimitiveComponent* InMeshComp);
	void SetCheckByBoxTrace();
	void EndCheck();
	void CheckDamage(bool InForceUseBoxTrace =false);
	void StartCheck(const FDamageInfo& InDamageInfo) { DamageInfo = InDamageInfo; }
protected:
	void CheckDamageByBoxTrace();
	
	UFUNCTION(Client, Reliable)
	void CheckDamageByMesh();

	UFUNCTION(Server, Reliable)
	void Server_CheckDamge(const TArray<FVector>& InSocketsLocations);
protected:
	UPROPERTY()
	FDamageInfo DamageInfo;
	
	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	FVector BoxHalfSize = FVector(50, 50, 50);

	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	float MeshCheckHalfHeight = 20.f;
	
	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	float MeshCheckRadius = 20.f;

	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectType
	{
		EObjectTypeQuery::ObjectTypeQuery3, EObjectTypeQuery::ObjectTypeQuery6
	};

	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceType = EDrawDebugTrace::None;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CheckMeshComp;

	UPROPERTY()
	bool bCheckByMeshComp = false;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> HitActors;
};
