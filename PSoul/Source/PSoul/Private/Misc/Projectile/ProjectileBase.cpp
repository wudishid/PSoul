// Fill out your copyright notice in the Description page of Project Settings.
#include "Misc/Projectile/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/ProjectilePoolSubsystem.h"
#include "Util/Util_Common.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	
	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMoveComp->SetIsReplicated(true);
	ProjectileMoveComp->InitialSpeed = 600.f;
	ProjectileMoveComp->MaxSpeed = 1200.f;
	ProjectileMoveComp->bRotationFollowsVelocity = true;
}

void AProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Clients have to park the instance as well, and bActorEnableCollision - which parking disables - is not
	// itself replicated, so drive their state from this flag instead.
	DOREPLIFETIME(AProjectileBase, bProjectileActive);
}

// Defined here rather than inline: TWeakObjectPtr needs UProjectilePoolSubsystem to be complete.
UProjectilePoolSubsystem* AProjectileBase::GetOwningPool() const
{
	return OwningPool.Get();
}

void AProjectileBase::SetOwningPool(UProjectilePoolSubsystem* InPool)
{
	OwningPool = InPool;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(ProjectileLifeSpan);
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UProjectilePoolSubsystem* Pool = OwningPool.Get())
	{
		// Keeps the pool from holding a dangling pointer if something destroys us behind its back.
		Pool->NotifyProjectileDestroyed(this);
	}
	OwningPool.Reset();

	Super::EndPlay(EndPlayReason);
}

void AProjectileBase::LifeSpanExpired()
{
	if (UProjectilePoolSubsystem* Pool = OwningPool.Get())
	{
		// Recycle instead of destroying: the pool re-arms the timer on the next launch, so a projectile that
		// merely missed everything costs nothing to fire again.
		Pool->ReleaseProjectile(this);
		return;
	}

	Super::LifeSpanExpired();
}

void AProjectileBase::ActivateProjectile(const FTransform& SpawnTransform, AActor* InOwner, APawn* InInstigator)
{
	bActivationInProgress = true;
	bReleaseRequested = false;

	SetOwner(InOwner);
	SetInstigator(InInstigator);

	// A reused projectile is teleported a long way, so never sweep it into place.
	SetActorLocationAndRotation(SpawnTransform.GetLocation(), SpawnTransform.Rotator(), /*bSweep=*/false, nullptr, ETeleportType::TeleportPhysics);

	// Set before collision comes back on so the very first overlap counts as live.
	bProjectileActive = true;

	if (ProjectileMoveComp)
	{
		// UProjectileMovementComponent::StopSimulating() nulls UpdatedComponent, and
		// SetVelocityInLocalSpace() needs it, so re-establish it before touching the velocity.
		ProjectileMoveComp->SetUpdatedComponent(RootComponent);

		// Mirrors what InitializeComponent() does, but only once at spawn. A recycled instance would
		// otherwise keep the velocity it died with, or none at all if it stopped on impact.
		const float LaunchSpeed = (ProjectileMoveComp->InitialSpeed > 0.f) ? ProjectileMoveComp->InitialSpeed : ProjectileMoveComp->MaxSpeed;
		ProjectileMoveComp->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
		ProjectileMoveComp->Activate(true);
		ProjectileMoveComp->UpdateComponentVelocity();
	}

	// Re-arm the recycle timer that DeactivateProjectile() cleared.
	SetLifeSpan(ProjectileLifeSpan);

	// Wake the actor so clients get the new position now rather than on some later dormancy flush.
	SetNetDormancy(DORM_Awake);
	ForceNetUpdate();

	// Deliberately last: re-enabling collision can immediately fire an overlap that releases this projectile
	// again, so every other piece of launch state must already be settled by the time that can happen.
	ApplyProjectileActiveState(true);

	bActivationInProgress = false;

	// That overlap may have asked to park us. Honour it now that the launch is complete.
	if (bReleaseRequested)
	{
		bReleaseRequested = false;
		ReleaseToPool();
	}
}

void AProjectileBase::DeactivateProjectile()
{
	bProjectileActive = false;

	// The pool owns the actor now; without clearing this the lifespan timer would destroy it while parked.
	SetLifeSpan(0.f);

	if (ProjectileMoveComp)
	{
		ProjectileMoveComp->StopMovementImmediately();
	}

	if (HasAuthority())
	{
		// Drop the shooter so a parked projectile is not tied to the lifetime of whoever fired it.
		SetOwner(nullptr);
		SetInstigator(nullptr);

		// A parked projectile neither moves nor changes again, so take it out of replication entirely and
		// push the final hidden state exactly once.
		SetNetDormancy(DORM_DormantAll);
		FlushNetDormancy();
	}

	ApplyProjectileActiveState(false);

	// Replaces the Event Destroyed signal that a pooled projectile no longer produces.
	OnProjectileDeactivated();
}

void AProjectileBase::ApplyProjectileActiveState(bool bActive)
{
	if (ProjectileMoveComp)
	{
		ProjectileMoveComp->SetComponentTickEnabled(bActive);
	}

	SetActorTickEnabled(bActive);
	SetActorHiddenInGame(!bActive);

	// Collision is toggled last on purpose. SetActorEnableCollision early-outs when the value has not changed,
	// so a freshly spawned projectile never re-runs UpdateOverlaps and cannot apply its damage twice. When it
	// does change, re-enabling it can fire an immediate overlap - which releases the projectile again - so
	// every other piece of launch state has to be settled before that can happen.
	SetActorEnableCollision(bActive);
}

void AProjectileBase::OnRep_ProjectileActive()
{
	// Clients never run the spawn or park logic, so mirror the server's state onto their own copy.
	ApplyProjectileActiveState(bProjectileActive);
}

void AProjectileBase::ReleaseToPool()
{
	if (bActivationInProgress)
	{
		// Reached from an overlap that fired while the projectile was still being launched. Let the launch
		// finish so the pool never sees a half-initialised instance, then park it on the way out.
		bReleaseRequested = true;
		return;
	}

	if (UProjectilePoolSubsystem* Pool = OwningPool.Get())
	{
		Pool->ReleaseProjectile(this);
		return;
	}

	Destroy();
}

void AProjectileBase::OnProjectileOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// A parked projectile keeps its components, so a stale overlap can still arrive after it was released.
	if (!bProjectileActive || !HasAuthority())
	{
		return;
	}

	if (OtherActor && OtherActor != GetOwner())
	{
		if (Util_Common::ApplyDamage(GetOwner(), OtherActor, DamageInfo))
		{
			ReleaseToPool();
		}
	}
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
