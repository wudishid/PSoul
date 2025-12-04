// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DamageCheckComponent.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/GameplayEffect/DamageGameplayEffectComponent.h"
#include "Interface/SoulDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/SoulGameFunctionLibrary.h"
#include "Util/Util_Common.h"


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
			for (const FHitResult& Hit : Hits)
			{
				if (AActor* HitActor = Hit.GetActor())
				{
					if (!HitActors.Contains(HitActor))
					{
						HitActors.AddUnique(HitActor);
						
						Util_Common::ApplyDamage(GetOwner(), HitActor, DamageInfo);
					}
				}
			}
		}
	}
}

void UDamageCheckComponent::Server_CheckDamge_Implementation(const TArray<FVector>& InSocketsLocations)
{
	TArray<FHitResult> Hits;
	TArray ActorsToIgnore{GetOwner()};
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
			for (const FHitResult& Hit : Hits)
			{
				if (AActor* HitActor = Hit.GetActor())
				{
					if (!HitActors.Contains(HitActor))
					{
						HitActors.AddUnique(HitActor);
						
						Util_Common::ApplyDamage(GetOwner(), HitActor, DamageInfo);
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




