// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/SoulGameFunctionLibrary.h"

#include "GameFramework/SoulCharacterBase.h"
#include "Util/Util_Common.h"

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
	return Util_Common::SpawnInventroyItemInstance(OwnerActor, ItemClass);
}
