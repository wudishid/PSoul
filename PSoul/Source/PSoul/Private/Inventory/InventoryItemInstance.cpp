// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemInstance.h"
#include "Components/BoxComponent.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "PSoul/SoulLog.h"
#include "Util/Util_Inventory.h"

// Sets default values
AInventoryItemInstance::AInventoryItemInstance()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	
	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
	ItemMeshComp->SetupAttachment(GetRootComponent());

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(GetRootComponent());
	BoxComp->SetHiddenInGame(false);
	
	bReplicates = true;
}


// Called when the game starts or when spawned
void AInventoryItemInstance::BeginPlay()
{
	Super::BeginPlay();
	
	if(Util_Inventory::GetItemInfoByName(ItemName, ItemInfo))
	{
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		if(!BoxComp->OnComponentBeginOverlap.IsAlreadyBound(this, &ThisClass::OnBoxCompOverlap))
		{
			BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxCompOverlap);
		}
	}
	else
	{
		ItemMeshComp->SetStaticMesh(nullptr);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(BoxComp->OnComponentBeginOverlap.IsAlreadyBound(this, &ThisClass::OnBoxCompOverlap))
		{
			BoxComp->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnBoxCompOverlap);
		}
		UE_LOG(LogSoulInventory, Warning, TEXT("ItemInfo not found!"));
	}
}

void AInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void AInventoryItemInstance::OnBoxCompOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(UInventoryManagerComponent* InventoryManagerComponent = OtherActor->FindComponentByClass<UInventoryManagerComponent>())
		{
			InventoryManagerComponent->AddItem(ItemInfo);
			Destroy();
		}
	}
}



