// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Equipment_Weapon.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/GameplayAbility/GameplayAbility_CombAttack.h"
#include "PSoul/SoulGameplayTags.h"


// Sets default values
AEquipment_Weapon::AEquipment_Weapon()
{
	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	WeaponMeshComp->SetIsReplicated(true);
	RootComponent = WeaponMeshComp;
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

	for (auto& Ability : AbilitiesToGive)
	{
		FGameplayAbilitySpec Spec(Ability);
		if (Ability->GetDefaultObject<USoulGameplayAbility>()->GetActivationPolicy() == ESoulAbilityActivationPolicy::OnInputTriggered)
		{
			Spec.DynamicAbilityTags.AddTag(SoulGameplayTags::InputTag_Attack);
		}
		AbilitiesSpecHandle.Add(ASC->GiveAbility(Spec));
	}
	
}

void AEquipment_Weapon::UnEquip()
{
	Super::UnEquip();

	for (auto& AbilityHandle : AbilitiesSpecHandle)
	{
		ASC->ClearAbility(AbilityHandle);
	}
	
	Destroy();
}

