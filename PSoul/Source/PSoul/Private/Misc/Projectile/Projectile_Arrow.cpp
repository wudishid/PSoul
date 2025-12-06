// Fill out your copyright notice in the Description page of Project Settings.
#include "Misc/Projectile/Projectile_Arrow.h"
#include "Components/BoxComponent.h"
#include "Util/Util_Common.h"


// Sets default values
AProjectile_Arrow::AProjectile_Arrow()
{
	CheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckBox"));
	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileOverlap);
	CheckBox->SetupAttachment(RootComponent);
	
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMeshComp"));
	ArrowMesh->SetIsReplicated(true);
	ArrowMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile_Arrow::BeginPlay()
{
	Super::BeginPlay();
}

