// Fill out your copyright notice in the Description page of Project Settings.
#include "Misc/SoulGameFunctionLibrary.h"

#include "Components/QuickSkillManager.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "GameFramework/SoulCharacterBase.h"
#include "GAS/SoulAbilitySystemComponent.h"
#include "GAS/Attribute/SoulCharacterSet.h"
#include "GAS/Attribute/SoulPlayerSet.h"
#include "GAS/GameplayAbility/SoulGameplayAbility.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Save/SoulSaveGame_PlayerData.h"
#include "SkillTreeSystem/SkillTreeManager.h"
#include "Util/Util_Common.h"

#define SAVE_SLOT TEXT("SoulGame1")


bool USoulGameFunctionLibrary::IsSameTeam(AActor* InActor1, AActor* InActor2)
{
	IGenericTeamAgentInterface* TeamAgent1 = Cast<IGenericTeamAgentInterface>(InActor1);
	if (TeamAgent1)
	{
		return TeamAgent1->GetTeamAttitudeTowards(*InActor2) != ETeamAttitude::Hostile;
	}
	return false;
}

bool USoulGameFunctionLibrary::SpawnInventroyItemInstance(AActor* OwnerActor,
	TSubclassOf<AInventoryItemInstance> ItemClass)
{
	return Util_Common::SpawnInventoryItemInstance(OwnerActor, ItemClass);
}

EEventDirection USoulGameFunctionLibrary::GetEventDataDirection(const FGameplayEventData& InEventData,
                                                                USoulGameplayAbility* InAbility)
{
	if (InAbility && InEventData.Instigator)
	{
		FVector SelfLoction = InAbility->GetOwningActorFromActorInfo()->GetActorLocation();
		FVector TargetLocation = InEventData.Instigator->GetActorLocation();
		float dotValue = (TargetLocation - SelfLoction).GetSafeNormal().Dot(
			InAbility->GetOwningActorFromActorInfo()->GetActorForwardVector());
		if (dotValue > 0)
		{
			return EEventDirection::Forward;
		}
		else
		{
			return EEventDirection::Back;
		}
	}

	return EEventDirection::Forward;
}

void USoulGameFunctionLibrary::SaveGame(APawn* InPawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("SaveGame..."));

	USoulAbilitySystemComponent* ASC = InPawn->FindComponentByClass<USoulAbilitySystemComponent>();
	check(ASC);
	UInventoryManagerComponent* InventoryManagerComp = InPawn->FindComponentByClass<UInventoryManagerComponent>();
	check(InventoryManagerComp);
	UEquipmentManagerComponent* EquipmentManagerComp = InPawn->FindComponentByClass<UEquipmentManagerComponent>();
	check(EquipmentManagerComp);
	USkillTreeManager* SkillTreeManagerComp = InPawn->FindComponentByClass<USkillTreeManager>();
	check(SkillTreeManagerComp);
	UQuickSkillManager* QuickSkillManagerComp = InPawn->GetController()->FindComponentByClass<UQuickSkillManager>();
	check(QuickSkillManagerComp);
	
	auto SaveGameFunc = [ASC, InventoryManagerComp, EquipmentManagerComp, SkillTreeManagerComp, QuickSkillManagerComp](USoulSaveGame_PlayerData* SaveGame_PlayerData)
	{
		//属性数据
		SaveGame_PlayerData->SavedData.CharacterData.Health.SetCurrentValue(
			ASC->GetSet<USoulCharacterSet>()->GetHealth());
		SaveGame_PlayerData->SavedData.CharacterData.Stamina.SetCurrentValue(
			ASC->GetSet<USoulCharacterSet>()->GetStamina());
		
		SaveGame_PlayerData->SavedData.PlayerData.Level.SetCurrentValue(ASC->GetSet<USoulPlayerSet>()->GetLevel());
		SaveGame_PlayerData->SavedData.PlayerData.NextLevelNeedSoul.SetCurrentValue(
			ASC->GetSet<USoulPlayerSet>()->GetNextLevelNeedSoul());
		SaveGame_PlayerData->SavedData.PlayerData.Life.SetCurrentValue(ASC->GetSet<USoulPlayerSet>()->GetLife());
		SaveGame_PlayerData->SavedData.PlayerData.Strength.
		                     SetCurrentValue(ASC->GetSet<USoulPlayerSet>()->GetStrength());
		SaveGame_PlayerData->SavedData.PlayerData.Stamina.SetCurrentValue(ASC->GetSet<USoulPlayerSet>()->GetStamina());
		SaveGame_PlayerData->SavedData.PlayerData.Soul.SetCurrentValue(ASC->GetSet<USoulPlayerSet>()->GetSoul());

		//物品数据
		if (FInventoryItemSlotList* InventoryItemSlotList = InventoryManagerComp->GetInventoryItemSlotList())
		{
			//先清空再加！
			SaveGame_PlayerData->SavedData.InventoryItemsData.Empty();
			for (const FInventoryItemSlot& Slot : InventoryItemSlotList->Slots)
			{
				SaveGame_PlayerData->SavedData.InventoryItemsData.Add(
					FSavedSingleItemData(Slot.ItemInfo.ItemName, Slot.Amount));
			}
		}

		//装备数据
		if (FEquipmentSlotList* EquipmentSlotList = EquipmentManagerComp->GetEquipmentSlotList())
		{
			//先清空再加！
			SaveGame_PlayerData->SavedData.WornItemData.Empty();
			for (FEquipmentSlot& Slot : EquipmentSlotList->Slots)
			{
				if (Slot.EquipmentInstance)
				{
					SaveGame_PlayerData->SavedData.WornItemData.Add(Slot.EquipmentInstance->GetItemName());
				}
			}
		}
		
		//技能树数据
		SaveGame_PlayerData->SavedData.UnlockedSkills = SkillTreeManagerComp->GetUnlockedSkills();
		SaveGame_PlayerData->SavedData.LearnedSkills = SkillTreeManagerComp->GetLearnedSkills();
		
		//快捷技能数据
		SaveGame_PlayerData->SavedData.QuickSkills = QuickSkillManagerComp->GetQuickSkillSlots();
		
		UGameplayStatics::SaveGameToSlot(SaveGame_PlayerData, SAVE_SLOT, 0);
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("SaveGame Finished!"));
	};
	
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT, 0))
	{
		if (USoulSaveGame_PlayerData* SaveGame_PlayerData = Cast<USoulSaveGame_PlayerData>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT, 0)))
		{
			SaveGameFunc(SaveGame_PlayerData);
		}
	}
	else
	{
		if (USoulSaveGame_PlayerData* SaveGame_PlayerData = Cast<USoulSaveGame_PlayerData>(UGameplayStatics::CreateSaveGameObject(USoulSaveGame_PlayerData::StaticClass())))
		{
			SaveGameFunc(SaveGame_PlayerData);
		}
	}
}

USoulSaveGame_PlayerData* USoulGameFunctionLibrary::LoadGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("Load Game..."));

	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT, 0))
	{
		return Cast<USoulSaveGame_PlayerData>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT, 0));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("No Save!"));
	}
	
	return nullptr;
}

void USoulGameFunctionLibrary::DeleteGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("Delete Game..."));

	if (UGameplayStatics::DeleteGameInSlot(SAVE_SLOT, 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("Delete Successful!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("Delete Failed!"));
	}
	
}
