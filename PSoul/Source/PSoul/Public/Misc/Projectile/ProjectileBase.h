// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS/GameplayEffect/SoulGameplayEffect_Damage.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UProjectilePoolSubsystem;

UCLASS(NotBlueprintable)
class PSOUL_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	/** True while this projectile is in flight, false while it sits parked in a pool. */
	UFUNCTION(BlueprintPure, Category = "Projectile")
	bool IsProjectileActive() const { return bProjectileActive; }

	/** The pool that owns this projectile, or null when it was not created through one. */
	UProjectilePoolSubsystem* GetOwningPool() const;

	/** Called by the pool immediately after spawning. */
	void SetOwningPool(UProjectilePoolSubsystem* InPool);

	/**
	 * Launches this projectile from SpawnTransform.
	 *
	 * Safe on a freshly spawned instance and on a recycled one: it re-seeds the movement state that
	 * UProjectileMovementComponent::InitializeComponent() only ever sets up once, so a reused projectile
	 * does not keep the velocity it died with.
	 */
	void ActivateProjectile(const FTransform& SpawnTransform, AActor* InOwner, APawn* InInstigator);

	/** Makes this projectile inert: no collision, no tick, hidden and network dormant. */
	void DeactivateProjectile();

	/** Hands this projectile back to the pool that owns it, or destroys it when it is not pooled. */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void ReleaseToPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Recycles the projectile into its pool instead of destroying it. */
	virtual void LifeSpanExpired() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Mirrors the server's active state onto this copy's visibility, collision and ticking. */
	UFUNCTION()
	void OnRep_ProjectileActive();

	/**
	 * Blueprint hook fired once the projectile has been parked and is inert again.
	 *
	 * A pooled projectile is no longer destroyed when it expires or lands, so anything that used to hang off
	 * Event Destroyed (trail teardown, attached effects, timers) belongs here instead. Runs on the authority.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
	void OnProjectileDeactivated();

	/** Applies the visibility, collision and tick state that matches bActive. */
	void ApplyProjectileActiveState(bool bActive);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float ProjectileLifeSpan = 15.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMoveComp;

	UPROPERTY(EditAnywhere, Category = "DamageCheck")
	FDamageInfo DamageInfo;

	/**
	 * Replicated because clients must park the instance too, and bActorEnableCollision - which the parking
	 * routine turns off - is not itself replicated.
	 */
	UPROPERTY(ReplicatedUsing = OnRep_ProjectileActive)
	bool bProjectileActive = true;

	/** The pool that owns this projectile. Weak so it cannot outlive a torn down world. */
	TWeakObjectPtr<UProjectilePoolSubsystem> OwningPool;

	/** True while ActivateProjectile is running; see ReleaseToPool. */
	bool bActivationInProgress = false;

	/** Set when ReleaseToPool is called during activation, and honoured once activation completes. */
	bool bReleaseRequested = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
