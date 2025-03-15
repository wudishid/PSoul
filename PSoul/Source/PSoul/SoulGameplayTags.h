// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NativeGameplayTags.h"

namespace SoulGameplayTags
{
	PSOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move)
	PSOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look)
	PSOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack)
	PSOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump)


	PSOUL_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Hit);
	PSOUL_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);

	PSOUL_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Hit);
	PSOUL_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	
}

