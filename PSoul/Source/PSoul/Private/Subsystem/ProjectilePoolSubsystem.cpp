// Fill out your copyright notice in the Description page of Project Settings.
#include "Subsystem/ProjectilePoolSubsystem.h"

#include "Engine/World.h"
#include "Misc/Projectile/ProjectileBase.h"

bool UProjectilePoolSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	// Only real gameplay worlds need a projectile pool; editor previews and the like do not.
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UProjectilePoolSubsystem::Deinitialize()
{
	// The actors themselves are torn down with the world, so just drop the bookkeeping and never touch
	// them from EndPlay during teardown.
	Buckets.Empty();
	OwnedProjectiles.Empty();

	Super::Deinitialize();
}

AProjectileBase* UProjectilePoolSubsystem::AcquireProjectile(TSubclassOf<AProjectileBase> ProjectileClass, const FTransform& SpawnTransform, AActor* InOwner, APawn* InInstigator)
{
	if (!ProjectileClass)
	{
		return nullptr;
	}

	FPoolBucket& Bucket = FindOrAddBucket(ProjectileClass.Get());

	// Take the most recently parked instance so its memory stays warm. Entries whose actor died behind the
	// pool's back are skipped rather than handed out.
	AProjectileBase* Projectile = nullptr;
	while (Bucket.Idle.Num() > 0)
	{
		AProjectileBase* Candidate = Bucket.Idle.Pop(EAllowShrinking::No);
		if (IsValid(Candidate))
		{
			Projectile = Candidate;
			break;
		}
	}

	if (!Projectile)
	{
		// Pool is empty, so this shot pays for a real spawn. Every later shot reuses it.
		UWorld* World = GetWorld();
		if (!World)
		{
			return nullptr;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = InOwner;
		SpawnParams.Instigator = InInstigator;
		// Deterministic placement: never let spawn collision handling relocate a projectile off its spawn point.
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTransform, SpawnParams);
		if (!Projectile)
		{
			return nullptr;
		}

		Projectile->SetOwningPool(this);
		OwnedProjectiles.Add(Projectile);
		++TotalSpawned;
	}

	// Account for the checkout before launching. Activating re-enables collision, which can immediately fire an
	// overlap that releases the projectile again; that nested release has to see the active count this checkout
	// just added. The instance may therefore already be parked again by the time this returns.
	++Bucket.Active;
	++TotalAcquired;

	// A freshly spawned projectile already had this applied by InitializeComponent; replaying it is a no-op
	// and lets both paths share one launch routine.
	Projectile->ActivateProjectile(SpawnTransform, InOwner, InInstigator);

	return Projectile;
}

void UProjectilePoolSubsystem::ReleaseProjectile(AProjectileBase* Projectile)
{
	if (!IsValid(Projectile))
	{
		return;
	}

	FPoolBucket& Bucket = FindOrAddBucket(Projectile->GetClass());

	// Guard against a double release: an already parked actor must not be queued twice.
	if (Bucket.Idle.Contains(Projectile))
	{
		return;
	}

	Bucket.Active = FMath::Max(Bucket.Active - 1, 0);

	// Park it first: this clears bProjectileActive, which is how NotifyProjectileDestroyed tells an instance
	// that has already been accounted for from one that is being destroyed while still checked out.
	Projectile->DeactivateProjectile();

	if (Bucket.Idle.Num() >= MaxIdlePerClass)
	{
		// At capacity, so let the surplus actor go rather than keep it resident. Its EndPlay calls back into
		// NotifyProjectileDestroyed, which is what unregisters it.
		Projectile->Destroy();
		return;
	}

	Bucket.Idle.Add(Projectile);
}

void UProjectilePoolSubsystem::NotifyProjectileDestroyed(AProjectileBase* Projectile)
{
	if (!Projectile)
	{
		return;
	}

	if (FPoolBucket* Bucket = Buckets.Find(Projectile->GetClass()))
	{
		// Only a still-active instance counts against the active tally; a parked one was already accounted for
		// when it was released.
		if (Projectile->IsProjectileActive())
		{
			Bucket->Active = FMath::Max(Bucket->Active - 1, 0);
		}

		Bucket->Idle.RemoveSingleSwap(Projectile, EAllowShrinking::No);
	}

	OwnedProjectiles.Remove(Projectile);
}

int32 UProjectilePoolSubsystem::GetIdleCount(TSubclassOf<AProjectileBase> ProjectileClass) const
{
	const FPoolBucket* Bucket = FindBucket(ProjectileClass.Get());
	return Bucket ? Bucket->Idle.Num() : 0;
}

int32 UProjectilePoolSubsystem::GetActiveCount(TSubclassOf<AProjectileBase> ProjectileClass) const
{
	const FPoolBucket* Bucket = FindBucket(ProjectileClass.Get());
	return Bucket ? Bucket->Active : 0;
}

UProjectilePoolSubsystem::FPoolBucket& UProjectilePoolSubsystem::FindOrAddBucket(UClass* ProjectileClass)
{
	if (FPoolBucket* Existing = Buckets.Find(ProjectileClass))
	{
		return *Existing;
	}

	return Buckets.Add(ProjectileClass, FPoolBucket());
}

const UProjectilePoolSubsystem::FPoolBucket* UProjectilePoolSubsystem::FindBucket(UClass* ProjectileClass) const
{
	return ProjectileClass ? Buckets.Find(ProjectileClass) : nullptr;
}
