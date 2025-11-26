// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DamageCheckComponent.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/GameplayEffect/DamageGameplayEffectComponent.h"
#include "Interface/SoulDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/SoulGameFunctionLibrary.h"



// Sets default values for this component's properties
UDamageCheckComponent::UDamageCheckComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UDamageCheckComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UDamageCheckComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UDamageCheckComponent::SetCheckByMesh(UPrimitiveComponent* InMeshComp)
{
	CheckMeshComp = InMeshComp;
	bCheckByMeshComp = true;
}

void UDamageCheckComponent::SetCheckByBoxTrace()
{
	CheckMeshComp = nullptr;
	bCheckByMeshComp = false;
}

void UDamageCheckComponent::EndCheck()
{
	HitActors.Empty();
}

void UDamageCheckComponent::CheckDamage(bool InForceUseBoxTrace)
{
	if (InForceUseBoxTrace)
	{
		CheckDamageByBoxTrace();
	}
	else if (bCheckByMeshComp)
	{
		CheckDamageByMesh();
	}
	else
	{
		CheckDamageByBoxTrace();
	}
}

void UDamageCheckComponent::CheckDamageByBoxTrace()
{
	if (ASoulCharacterBase* OwnerCharacter = Cast<ASoulCharacterBase>(GetOwner()))
	{
		FVector OwnerLocation = OwnerCharacter->GetActorLocation();
		//检测盒子向前偏移
		FVector StartPos = OwnerLocation + (OwnerCharacter->GetActorForwardVector() * (BoxHalfSize.X * 0.7f));
		FVector EndPos = StartPos + OwnerCharacter->GetActorForwardVector();
		TArray<AActor*> ActorsToIgnore{OwnerCharacter};
		TArray<FHitResult> Hits;
		UKismetSystemLibrary::BoxTraceMultiForObjects(OwnerCharacter->GetWorld(), StartPos, EndPos, BoxHalfSize,
		                                              OwnerCharacter->GetActorRotation(), TraceObjectType, false,
		                                              ActorsToIgnore, DrawDebugTraceType, Hits, true);

		if (!Hits.IsEmpty())
		{
			USoulAbilitySystemComponent* CauserASC = GetOwner()->FindComponentByClass<USoulAbilitySystemComponent>();
			if (!CauserASC) return;
			
			for (const FHitResult& Hit : Hits)
			{
				if (AActor* HitActor = Hit.GetActor())
				{
					if (!HitActors.Contains(HitActor))
					{
						HitActors.AddUnique(HitActor);

						if (ISoulDamageInterface* DamageInterface = Cast<ISoulDamageInterface>(HitActor))
						{
							DamageInterface->Execute_TakeDamage(HitActor);
						}
						
						if (USoulGameFunctionLibrary::IsSameTeam(HitActor, GetOwner())) continue;
						
						USoulAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<
							USoulAbilitySystemComponent>();
						if (!TargetASC) continue;

						if (DamageInfo.DamageEffect)
						{
							UGameplayEffect* GameplayEffect = DamageInfo.DamageEffect.GetDefaultObject();
							UDamageGameplayEffectComponent& DamageGameplayEffectComponent = GameplayEffect->
								FindOrAddComponent<UDamageGameplayEffectComponent>();
							DamageGameplayEffectComponent.Impulse = DamageInfo.DamageImpulse;
							CauserASC->ApplyGameplayEffectToTarget(GameplayEffect, TargetASC, 1);
						}
					}
				}
			}
		}
	}
}

void UDamageCheckComponent::Server_CheckDamge_Implementation(const TArray<FVector>& InSocketsLocations)
{
	TArray<FHitResult> Hits;
	TArray ActorsToIgnore{CheckMeshComp->GetOwner()};
	for (int i = 0; i <InSocketsLocations.Num(); i++)
	{
		FVector SocketLocation = InSocketsLocations[i];
		UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), SocketLocation,
														  SocketLocation,
														  MeshCheckRadius, MeshCheckHalfHeight,
														  TraceObjectType, false, ActorsToIgnore,
														  DrawDebugTraceType, Hits, true);

		if (!Hits.IsEmpty())
		{
			USoulAbilitySystemComponent* CauserASC = GetOwner()->FindComponentByClass<USoulAbilitySystemComponent>();
			if (!CauserASC) return;
		
			for (const FHitResult& Hit : Hits)
			{
				if (AActor* HitActor = Hit.GetActor())
				{
					if (!HitActors.Contains(HitActor))
					{
						HitActors.AddUnique(HitActor);
					
						if (ISoulDamageInterface* DamageInterface = Cast<ISoulDamageInterface>(HitActor))
						{
							DamageInterface->Execute_TakeDamage(HitActor);
						}

						if (USoulGameFunctionLibrary::IsSameTeam(HitActor, GetOwner())) continue;
					
						USoulAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<
								USoulAbilitySystemComponent>();
						if (!TargetASC) continue;

						if (DamageInfo.DamageEffect)
						{
							UGameplayEffect* GameplayEffect = DamageInfo.DamageEffect.GetDefaultObject();
							UDamageGameplayEffectComponent& DamageGameplayEffectComponent = GameplayEffect->
								FindOrAddComponent<UDamageGameplayEffectComponent>();
							DamageGameplayEffectComponent.Impulse = DamageInfo.DamageImpulse;
							CauserASC->ApplyGameplayEffectToTarget(GameplayEffect, TargetASC, 1);
						}
					}
				}
			}
		}
	}
}

void UDamageCheckComponent::CheckDamageByMesh_Implementation()
{
	if (CheckMeshComp)
	{
		TArray<FName> AllSocketNames = CheckMeshComp->GetAllSocketNames();
		TArray<FVector>SocketsLocations;
		for (FName SocketName : AllSocketNames)
		{
			SocketsLocations.Add(CheckMeshComp->GetSocketLocation(SocketName));
		}
		Server_CheckDamge(SocketsLocations);
	}
}




