// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/InventoryManagerComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
}

void UInventoryManagerComponent::RemoveItem_Implementation(int32 InItemIndex)
{
	if(InventorySlotList.Slots.IsValidIndex(InItemIndex))
	{
		InventorySlotList.Slots.RemoveAt(InItemIndex);
	}
}

void UInventoryManagerComponent::DropItem_Implementation(int32 InItemIndex)
{
	if(InventorySlotList.Slots.IsValidIndex(InItemIndex))
	{
		TSubclassOf<AInventoryItemInstance> ItemInstanceClass = InventorySlotList.Slots[InItemIndex].ItemInfo.ItemClass;
		if(ItemInstanceClass)
		{
			FActorSpawnParameters Sp;
			Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			// 1. 定义射线起点和终点（垂直向下）
			FVector StartLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100;; // 初始生成点
			FVector EndLocation = StartLocation + FVector(0, 0, -1000); // 向下发射1000单位

			// 2. 执行射线检测（仅检测地面通道）
			FHitResult HitResult;
			FCollisionQueryParams TraceParams;
			bool bHit = GetWorld()->LineTraceSingleByChannel(
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
				if(GetWorld()->SpawnActor<AInventoryItemInstance>(ItemInstanceClass, SpawnLocation, SpawnRotation, Sp))
				{
					RemoveItem(InItemIndex);
				}
			}
		}
	}
}

bool UInventoryManagerComponent::GetItemInfoByIndex(int32 Index, FInventoryItemInfo& OutItemInfo) const
{
	if(InventorySlotList.Slots.IsValidIndex(Index))
	{
		OutItemInfo = InventorySlotList.Slots[Index].ItemInfo;
		return true;
	}

	return false;
}

bool UInventoryManagerComponent::GetItemSlotByIndex(int32 Index, FInventoryItemSlot& OutItemSlot) const
{
	if(InventorySlotList.Slots.IsValidIndex(Index))
	{
		OutItemSlot = InventorySlotList.Slots[Index];
		return true;
	}

	return false;
}

void UInventoryManagerComponent::AddItem_Implementation(const FInventoryItemInfo& ItemInfo)
{
	bool FinishAdd = false;
	
	for(FInventoryItemSlot& ItemSlot : InventorySlotList.Slots)
	{
		if(ItemSlot.ItemInfo == ItemInfo)
		{
			if(ItemSlot.ItemInfo.CanStack)
			{
				ItemSlot.Amount ++;
				FinishAdd = true;
			}
		}
	}

	if(FinishAdd)
	{
		InventorySlotList.MarkArrayDirty();
	}
	else
	{
		FInventoryItemSlot itemSlot;
		itemSlot.ItemInfo = ItemInfo;
		itemSlot.Amount = 1;
		InventorySlotList.Slots.Add(itemSlot);
		InventorySlotList.MarkArrayDirty();
	}
}


// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UInventoryManagerComponent, InventorySlotList, COND_None, REPNOTIFY_Always);
}

void UInventoryManagerComponent::OnRep_SlotList()
{
	OnInventorySlotListChanged.Broadcast(InventorySlotList);
}


