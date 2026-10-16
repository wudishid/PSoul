// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ProjectilePoolSubsystem.generated.h"

class AProjectileBase;

/**
 * Recycles AProjectileBase instances per world instead of spawning and destroying one per shot.
 *
 * Spawning an actor allocates a whole UObject graph, registers its components and replicates a brand new
 * actor to every client; destroying it tears all of that down again. Arrows and spell balls are short lived
 * and highly repetitive, so this pool parks used instances (hidden, collisionless and network dormant) and
 * hands them straight back out. Past the first few shots of a match, firing costs no allocation at all.
 *
 * Lives on the world rather than the game instance: pooled actors belong to exactly one world and are torn
 * down with it.
 */
UCLASS()
class PSOUL_API UProjectilePoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Returns an active projectile of ProjectileClass, reusing a parked instance when one is available.
	 * Spawns a new actor only when the pool is empty, so the steady state allocates nothing.
	 */
	AProjectileBase* AcquireProjectile(TSubclassOf<AProjectileBase> ProjectileClass, const FTransform& SpawnTransform, AActor* InOwner, APawn* InInstigator);

	/** Parks Projectile for reuse. Ignored when it is null, already parked, or not owned by this pool. */
	void ReleaseProjectile(AProjectileBase* Projectile);

	/** Called from AProjectileBase::EndPlay so the pool never keeps a dangling pointer. */
	void NotifyProjectileDestroyed(AProjectileBase* Projectile);

	/** How many instances of ProjectileClass are parked and ready to be handed out. */
	UFUNCTION(BlueprintPure, Category = "ProjectilePool")
	int32 GetIdleCount(TSubclassOf<AProjectileBase> ProjectileClass) const;

	/** How many instances of ProjectileClass are currently in flight. */
	UFUNCTION(BlueprintPure, Category = "ProjectilePool")
	int32 GetActiveCount(TSubclassOf<AProjectileBase> ProjectileClass) const;

	/** Actors this pool has actually spawned. Compare against GetTotalAcquiredCount for the reuse rate. */
	UFUNCTION(BlueprintPure, Category = "ProjectilePool")
	int32 GetTotalSpawnedCount() const { return TotalSpawned; }

	/** Successful AcquireProjectile calls, i.e. shots fired through the pool. */
	UFUNCTION(BlueprintPure, Category = "ProjectilePool")
	int32 GetTotalAcquiredCount() const { return TotalAcquired; }

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void Deinitialize() override;

	/** Cap on parked instances per class, so a burst cannot leave an unbounded pool resident. */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectilePool", meta = (UIMin = "0", ClampMin = "0"))
	int32 MaxIdlePerClass = 64;

private:
	/** Bookkeeping for a single projectile class. */
	struct FPoolBucket
	{
		/** Parked instances, most recently released last. */
		TArray<AProjectileBase*> Idle;

		/** Instances currently in flight. */
		int32 Active = 0;
	};

	FPoolBucket& FindOrAddBucket(UClass* ProjectileClass);
	const FPoolBucket* FindBucket(UClass* ProjectileClass) const;

	/**
	 * Every projectile this pool owns, parked or in flight. This array is the GC root for the raw pointers
	 * held in Buckets.
	 */
	UPROPERTY()
	TArray<TObjectPtr<AProjectileBase>> OwnedProjectiles;

	/** Parked instances per class. */
	TMap<UClass*, FPoolBucket> Buckets;

	int32 TotalSpawned = 0;
	int32 TotalAcquired = 0;
};
