// Fill out your copyright notice in the Description page of Project Settings.
#include "Misc/Projectile/Projectile_Arrow.h"
#include "Components/BoxComponent.h"
#include "Util/Util_Common.h"


// Sets default values
AProjectile_Arrow::AProjectile_Arrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckBox"));
	CheckBox->SetIsReplicated(true);
	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnArrowMeshCompOverlap);
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

void AProjectile_Arrow::OnArrowMeshCompOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                              AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                              int32 OtherBodyIndex, bool bFromSweep,
                                                              const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (OtherActor && OtherActor != GetOwner())
		{
			Util_Common::ApplyDamage(GetOwner(), OtherActor, DamageInfo);
		}
	}
}

// Called every frame
void AProjectile_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

