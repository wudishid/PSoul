// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIPopupManager.generated.h"


UENUM()
enum class EPopupWidgetLayer : uint8
{
	Normal			UMETA(DisplayName = "普通情况"),
	Inventory		UMETA(DisplayName = "背包面板"),
	Equipment		UMETA(DisplayName = "装备面板"),
	SkillTree		UMETA(DisplayName = "技能树面板"),
};



USTRUCT()
struct FPopupWidgetInfo
{
	GENERATED_BODY()

	FPopupWidgetInfo() : Layer(EPopupWidgetLayer::Normal){}
	FPopupWidgetInfo(EPopupWidgetLayer InLayer) : Layer(InLayer){}
	
	UPROPERTY()
	EPopupWidgetLayer Layer;

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> Children;

	bool operator==(const FPopupWidgetInfo& rhs) const
	{
		return Layer == rhs.Layer;
	}
};

UCLASS()
class PSOUL_API UUIPopupManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	template<typename WidgetT = UUserWidget>
	WidgetT* PopupPanelWidget(UUserWidget* InParent, EPopupWidgetLayer InLayer = EPopupWidgetLayer::Normal, TSubclassOf<UUserWidget> InPopupWidgetClass = WidgetT::StaticClass(), TFunction<void(WidgetT*)> InOperateFunc = nullptr)
	{
		if (ensureAlwaysMsgf(InParent, TEXT("ParentWidget is nullptr!")))
		{
			if (WidgetT* PopupWidget = CreateWidget<WidgetT>(InParent->GetOwningPlayer(), InPopupWidgetClass))
			{
				if (InOperateFunc != nullptr)
				{
					InOperateFunc(PopupWidget);
				}
				
				int32 Index = ActivePopupWidgetsMap.Find(InLayer);
				if (Index != INDEX_NONE)
				{
					ActivePopupWidgetsMap[Index].Children.Add(PopupWidget);
					return PopupWidget;
				}
				else
				{
					FPopupWidgetInfo PopupWidgetInfo;
					PopupWidgetInfo.Layer = InLayer;
					PopupWidgetInfo.Children.Add(PopupWidget);
					
					ActivePopupWidgetsMap.Add(PopupWidgetInfo);
					
					return PopupWidget;
				}
			}
		}
		return nullptr;
	}


	void RemoveAllPopupWidgetsByLayer(EPopupWidgetLayer InLayer)
	{
		for (auto& PopupWidgetInfo : ActivePopupWidgetsMap)
		{
			if (PopupWidgetInfo.Layer == InLayer)
			{
				for (auto& Child : PopupWidgetInfo.Children)
				{
					if (Child->IsInViewport())
					{
						Child->RemoveFromParent();
					}
				}
			}
		}
	}

protected:
	UPROPERTY()
	TArray<FPopupWidgetInfo> ActivePopupWidgetsMap
	{
		EPopupWidgetLayer::Normal,
		EPopupWidgetLayer::Inventory,
		EPopupWidgetLayer::Equipment,
		EPopupWidgetLayer::SkillTree,
	};
};

