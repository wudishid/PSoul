// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/Equipment_Weapon.h"


// Sets default values
AEquipment_Weapon::AEquipment_Weapon()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AEquipment_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipment_Weapon::Equip()
{
	Super::Equip();
	if(USkeletalMeshComponent* SkeMeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		GetRootComponent()->AttachToComponent(SkeMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}
}

void AEquipment_Weapon::UnEquip()
{
	Super::UnEquip();
	Destroy();
}

