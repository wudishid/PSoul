// Fill out your copyright notice in the Description page of Project Settings.
#include "Misc/Projectile/Projectile_Ball.h"
#include "Components/SphereComponent.h"


// Sets default values
AProjectile_Ball::AProjectile_Ball()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileOverlap);
	SphereComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile_Ball::BeginPlay()
{
	Super::BeginPlay();
}




