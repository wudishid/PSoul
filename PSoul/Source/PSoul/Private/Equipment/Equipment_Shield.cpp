// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Equipment_Shield.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "PSoul/SoulGameplayTags.h"
#include "GAS/GameplayAbility/GameplayAbility_Block.h"


// Sets default values
AEquipment_Shield::AEquipment_Shield()
{
	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ShieldMeshComp->SetIsReplicated(true);
	RootComponent = ShieldMeshComp;
}

// Called when the game starts or when spawned
void AEquipment_Shield::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipment_Shield::Equip()
{
	Super::Equip();
	
	if(USkeletalMeshComponent* SkeMeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		GetRootComponent()->AttachToComponent(SkeMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}

	FGameplayAbilitySpec Spec(BlockAbilityToGive);
	Spec.DynamicAbilityTags.AddTag(SoulGameplayTags::InputTag_Block);
	CombAttackAbilitySpecHandle = ASC->GiveAbility(Spec);
}

void AEquipment_Shield::UnEquip()
{
	Super::UnEquip();
	ASC->ClearAbility(CombAttackAbilitySpecHandle);
	Destroy();
}

