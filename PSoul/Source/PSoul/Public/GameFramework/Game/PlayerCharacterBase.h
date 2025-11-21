// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulCharacterBase.h"
#include "Interface/InteractInterface.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacterBase.generated.h"

class USkillTreeManager;
class ULockTargetComponent;
class USoulCameraComponent;
class UMotionWarpingComponent;
class AEquipmentInstance;
enum class EEquipmentType : uint8;
class UEquipmentManagerComponent;
class UInventoryManagerComponent;
class ASoulPlayerController_Game;
class USoulInputConfig;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS(config=Game)
class APlayerCharacterBase : public ASoulCharacterBase, public IInteractInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USoulCameraComponent* Camera;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USkillTreeManager* SkillTreeManagerComp;
	
public:
	APlayerCharacterBase();
	virtual FRotator GetDesiredRotation() const override;
	virtual void PickUpItem(const FInventoryItemInfo& ItemInfo) override;
protected:
	virtual void BeginPlay() override;
	
	virtual void HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance);
	virtual void HandleUnEquip(EEquipmentType InEquipmentType);
};

