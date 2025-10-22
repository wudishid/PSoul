// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DamageCheckComponent.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GameFramework/Game/SoulPlayerState_Game.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "PSoul/SoulGameplayTags.h"


// Sets default values for this component's properties
UDamageCheckComponent::UDamageCheckComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDamageCheckComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDamageCheckComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bCanCheck)
	{
		CheckDamage();
	}
}

void UDamageCheckComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDamageCheckComponent, CheckMeshComp);
	DOREPLIFETIME(UDamageCheckComponent, bCheckByMeshComp);
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

void UDamageCheckComponent::StartCheck()
{
	bCanCheck = true;
}

void UDamageCheckComponent::EndCheck()
{
	bCanCheck = false;
	LastSocketsLocation.Empty();
	HitActors.Empty();
}

void UDamageCheckComponent::CheckDamage()
{
	if(bCheckByMeshComp)
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
		FVector StartPos = OwnerCharacter->GetActorLocation();
		FVector EndPos = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector();
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

						USoulAbilitySystemComponent* CauserASC = OwnerCharacter->GetAbilitySystemComponent();
						if (!CauserASC) return;
						
						USoulAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<
							USoulAbilitySystemComponent>();
						if (!TargetASC) continue;
						
						UGameplayEffect* GameplayEffect = EffectToApply.GetDefaultObject();
						CauserASC->ApplyGameplayEffectToTarget(GameplayEffect, TargetASC, 1);
						
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

void UDamageCheckComponent::Server_CheckDamge_Implementation(const  TArray<FVector>& InSocketsLocations)
{
	TArray<FHitResult> HitResults;
	TArray ActorsToIgnore{CheckMeshComp->GetOwner()};
	for (int i = 0; i <InSocketsLocations.Num(); i++)
	{
		FVector SocketLocation = InSocketsLocations[i];
		UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), SocketLocation,
														  SocketLocation,
														  MeshCheckRadius, MeshCheckHalfHeight,
														  TraceObjectType, false, ActorsToIgnore,
														  DrawDebugTraceType, HitResults, true);
	}

	if (!HitResults.IsEmpty())
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (!HitActors.Contains(HitActor))
				{
					HitActors.AddUnique(HitActor);

					ASoulCharacterBase* OwnerCharacter = Cast<ASoulCharacterBase>(GetOwner());
					USoulAbilitySystemComponent* CauserASC = OwnerCharacter->GetAbilitySystemComponent();
					if (!CauserASC) return;
					
					USoulAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<
						USoulAbilitySystemComponent>();
					if (!TargetASC) continue;
					UGameplayEffect* GameplayEffect = EffectToApply.GetDefaultObject();
					CauserASC->ApplyGameplayEffectToTarget(GameplayEffect, TargetASC, 1);
				}
			}
		}
	}
}



