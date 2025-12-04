// Fill out your copyright notice in the Description page of Project Settings.

#include "Util/Util_Common.h"
#include "Development/Soul_CommonSetting.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/GameplayEffect/DamageGameplayEffectComponent.h"
#include "GAS/GameplayEffect/SoulGameplayEffect_Damage.h"
#include "Interface/SoulDamageInterface.h"
#include "Inventory/InventoryItemInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/SoulGameFunctionLibrary.h"

bool Util_Common::SpawnInventroyItemInstance(AActor* OwnerActor, TSubclassOf<AInventoryItemInstance> ItemClass)
{
	if (!OwnerActor) return false;
	if (!ItemClass) return false;
	UWorld* World = OwnerActor->GetWorld();
	if(ItemClass)
	{
		FActorSpawnParameters Sp;
		Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
		// 1. 定义射线起点和终点（垂直向下）
		FVector StartLocation = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * 100;; // 初始生成点
		FVector EndLocation = StartLocation + FVector(0, 0, -1000); // 向下发射1000单位

		// 2. 执行射线检测（仅检测地面通道）
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		bool bHit = World->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility, // 根据地面类型选择通道（如ECC_WorldStatic）
			TraceParams
		);

		// 3. 如果检测到地面，调整位置和旋转
		if (bHit)
		{
			FVector SpawnLocation = HitResult.Location;
			FRotator SpawnRotation = FRotationMatrix::MakeFromZ(HitResult.Normal).Rotator();

			// 生成物体
			if(World->SpawnActor<AInventoryItemInstance>(ItemClass, SpawnLocation, SpawnRotation, Sp))
			{
				return true;
			}
		}
	}

	return false;
}

void Util_Common::PlayOpenPanelSound(UWorld* InWorld)
{
	UGameplayStatics::PlaySound2D(InWorld, GetDefault<USoul_CommonSetting>()->OpenPanelSound.LoadSynchronous());
}

void Util_Common::PlayClosePanelSound(UWorld* InWorld)
{
	UGameplayStatics::PlaySound2D(InWorld, GetDefault<USoul_CommonSetting>()->ClosePanelSound.LoadSynchronous());
}

void Util_Common::ApplyDamage(AActor* InCauser, AActor* InTarget, const FDamageInfo& DamageInfo)
{
	if (InCauser && InTarget)
	{
		if (ISoulDamageInterface* DamageInterface = Cast<ISoulDamageInterface>(InTarget))
		{
			DamageInterface->Execute_TakeDamage(InTarget);
		}

		if (!GetDefault<USoul_CommonSetting>()->OpenFriendDamage)
		{
			if (USoulGameFunctionLibrary::IsSameTeam(InCauser, InTarget)) return;
		}
		
		USoulAbilitySystemComponent* CauserASC = InCauser->FindComponentByClass<USoulAbilitySystemComponent>();
		if (!CauserASC) return;
		USoulAbilitySystemComponent* TargetASC = InTarget->FindComponentByClass<
			USoulAbilitySystemComponent>();
		if (!TargetASC) return;

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

UTexture2D* Util_Common::GetBuffIconByBuffTag(FGameplayTag InBuffTag)
{
	if (GetDefault<USoul_CommonSetting>()->BuffsIconMap.Contains(InBuffTag))
	{
		return GetDefault<USoul_CommonSetting>()->BuffsIconMap.Find(InBuffTag)->LoadSynchronous();
	}
	
	return nullptr;
}

