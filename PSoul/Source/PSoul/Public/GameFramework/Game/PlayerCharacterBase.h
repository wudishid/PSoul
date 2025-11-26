// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SoulCharacterBase.h"
#include "Interface/InteractInterface.h"
#include "Interface/SkillReleaseControlInterface.h"
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
class APlayerCharacterBase : public ASoulCharacterBase, public IInteractInterface, public ISkillReleaseControlInterface
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillTreeManager")
	TObjectPtr<USkillTreeManager> SkillTreeManagerComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillControl")
	TObjectPtr<USceneComponent> SkillDirectionSceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillControl")
	TObjectPtr<UWidgetComponent> SkillDirectionWidgetComp;
public:
	APlayerCharacterBase();
	virtual FRotator GetDesiredRotation() const override;
	virtual void PickUpItem(FName InItemName, int32 Amount = 1) override;
	
	UFUNCTION()
	virtual void ApplyDirectionalSkillControl_Implementation() override;
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void HandleEquip(EEquipmentType InEquipmentType, AEquipmentInstance* EquipmentInstance);
	virtual void HandleUnEquip(EEquipmentType InEquipmentType);

protected:
	void UpdateDirectionalSkillControl();
private:
	bool bEnableDirectionalSkillControl = false;
};

