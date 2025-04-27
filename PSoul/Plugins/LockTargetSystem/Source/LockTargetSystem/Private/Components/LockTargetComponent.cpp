// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/LockTargetComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/LockPosComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
ULockTargetComponent::ULockTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULockTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnedCharacter=Cast<ACharacter>(GetOwner());
}


// Called every frame
void ULockTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(OwnedCharacter && OwnedCharacter->IsLocallyControlled())
	{
		ServerCheckCanLockTarget();
		UpdateCameraLockTarget();
	}
	
}

void ULockTargetComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULockTargetComponent, CurrentLockedTarget, COND_None, REPNOTIFY_Always);
}

void ULockTargetComponent::OnRep_CurrentLockTarget()
{
	if (IsLockingTarget())
	{
		OnLockStateChange.Broadcast(true);
	}
	else
	{
		OnLockStateChange.Broadcast(false);
	}
}

void ULockTargetComponent::TryLockTarget()
{
	if (IsLockingTarget())
	{
		ServerCancelLockTarget();
	}
	else
	{
		ServerLockTarget();
	}
}

void ULockTargetComponent::ServerLockTarget_Implementation()
{
	if(!IsLockingTarget())
	{
		//找到能被锁定的范围内的所有Actor
		TArray<AActor*> CanBeLockedTargets=FindCanBeLockedTargets();
		ClientFilterActorsAndLock(FindCanBeLockedTargets());
	}
}

void ULockTargetComponent::ServerCancelLockTarget_Implementation()
{
	ServerUnLockCurrentTarget();
}

void ULockTargetComponent::ServerLeftSwitchLockTarget_Implementation()
{
	if (CurrentLockedTarget)
	{
		//找到能被锁定的范围内的所有Actor
		TArray<AActor*> CanBeLockedTargets=FindCanBeLockedTargets();
		ClientLeftSwitchLockTarget(CanBeLockedTargets);
	}
}

void ULockTargetComponent::ServerRightSwitchLockTarget_Implementation()
{
	if (CurrentLockedTarget)
	{
		//找到能被锁定的范围内的所有Actor
		TArray<AActor*> CanBeLockedTargets=FindCanBeLockedTargets();
		ClientRightSwitchLockTarget(CanBeLockedTargets);
	}
}

bool ULockTargetComponent::IsLockingTarget() const
{
	return IsValid(CurrentLockedTarget);
}

TArray<AActor*> ULockTargetComponent::FindCanBeLockedTargets()
{
	TArray<AActor*> InRadiusTargets;
	TArray<AActor*> ResultTargets;
	
	//球形检测范围内的所有角色
	UKismetSystemLibrary::SphereOverlapActors
		(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		CheckRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>{EObjectTypeQuery::ObjectTypeQuery3},
		ACharacter::StaticClass(),
		TArray<AActor*>{GetOwner()},
		InRadiusTargets
		);

	//如果角色上挂有锁定点组件，则添加到结果数组
	for(AActor* InRadiusActor:InRadiusTargets)
	{
		ULockPosComponent* LockPosComponent=Cast<ULockPosComponent>(InRadiusActor->GetComponentByClass(ULockPosComponent::StaticClass()));
		
		if(LockPosComponent)
		{
			ResultTargets.Add(InRadiusActor);
		}
	}

	//返回结果数组
	return ResultTargets;
}

void ULockTargetComponent::ClientLeftSwitchLockTarget_Implementation(const TArray<AActor*>& InActors)
{
	//在屏幕内的Actors
	TArray<AActor*> InScreenActors=GetInScreenActors(InActors);
	
	//去掉当前锁定的目标
	if(InScreenActors.Contains(CurrentLockedTarget))
	{
		InScreenActors.Remove(CurrentLockedTarget);
	}

	FVector2D CurrentLockTargetScreenPos;
	UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(),0),CurrentLockedTarget->GetActorLocation(),CurrentLockTargetScreenPos);

	TArray<AActor*> CurrentLockTargetLeftActors;
	TArray<float> CurrentLockTargetLeftActorsDistance;
		
	for(AActor* InScreenActor:InScreenActors)
	{
		FVector2D InScreenActorPos;
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(),0),InScreenActor->GetActorLocation(),InScreenActorPos);
		if(InScreenActorPos.X<CurrentLockTargetScreenPos.X)
		{
			CurrentLockTargetLeftActors.Add(InScreenActor);

			//添加当前锁定Actor在屏幕的位置X减去此Actor在屏幕的位置X
			CurrentLockTargetLeftActorsDistance.Add(CurrentLockTargetScreenPos.X-InScreenActorPos.X);
		}
	}

	if(!CurrentLockTargetLeftActors.IsEmpty()&&!CurrentLockTargetLeftActorsDistance.IsEmpty())
	{
		int32 MinIndex;
		float MinValue;
		UKismetMathLibrary::MinOfFloatArray(CurrentLockTargetLeftActorsDistance,MinIndex,MinValue);

		if(MinIndex!=-1)
		{
			AActor* LeftSwitchTargetActor=CurrentLockTargetLeftActors[MinIndex];

			if(LeftSwitchTargetActor)
			{
				ServerSetActorToLock(LeftSwitchTargetActor);
			}
		}
	}
}

void ULockTargetComponent::ClientFilterActorsAndLock_Implementation(const TArray<AActor*>& InActors)
{
	//在屏幕内的Actors
	TArray<AActor*> InScreenActors=GetInScreenActors(InActors);
	
	//距离屏幕中心最近的Actor
	AActor* ClosestCenterScreenActor= GetClosestCenterScreenActor(InScreenActors);

	ServerSetActorToLock(ClosestCenterScreenActor);
}

void ULockTargetComponent::ClientRightSwitchLockTarget_Implementation(const TArray<AActor*>& InActors)
{
	//在屏幕内的Actors
	TArray<AActor*> InScreenActors=GetInScreenActors(InActors);
	
	//去掉当前锁定的目标
	if(InScreenActors.Contains(CurrentLockedTarget))
	{
		InScreenActors.Remove(CurrentLockedTarget);
	}

	//当前锁定Actor在屏幕的位置
	FVector2D CurrentLockTargetScreenPos;
	UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(),0),CurrentLockedTarget->GetActorLocation(),CurrentLockTargetScreenPos);

	TArray<AActor*> CurrentLockTargetRightActors;
	TArray<float> CurrentLockTargetRightActorsDistance;
		
	for(AActor* InScreenActor:InScreenActors)
	{
		FVector2D InScreenActorPos;
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(),0),InScreenActor->GetActorLocation(),InScreenActorPos);
		if(InScreenActorPos.X>CurrentLockTargetScreenPos.X)
		{
			CurrentLockTargetRightActors.Add(InScreenActor);
			//添加此Actor在屏幕的位置X减去当前锁定Actor在屏幕的位置X
			CurrentLockTargetRightActorsDistance.Add(InScreenActorPos.X-CurrentLockTargetScreenPos.X);
		}
	}

	if(!CurrentLockTargetRightActors.IsEmpty()&&!CurrentLockTargetRightActorsDistance.IsEmpty())
	{
		int32 MinIndex;
		float MinValue;
		UKismetMathLibrary::MinOfFloatArray(CurrentLockTargetRightActorsDistance,MinIndex,MinValue);

		if(MinIndex!=-1)
		{
			AActor* RightSwitchTargetActor=CurrentLockTargetRightActors[MinIndex];

			if(RightSwitchTargetActor)
			{
				ServerSetActorToLock(RightSwitchTargetActor);
			}
		}
	}
}

TArray<AActor*> ULockTargetComponent::GetInScreenActors(const TArray<AActor*>& InActors)
{

	//记录在视口里的Actor
	TArray<AActor*> InScreenActors;
	
	if(InActors.Num()>0)
	{
		for (AActor* FilterActor:InActors)
		{
			FVector2D InScreenPosition;
			
			//在视口的位置
			UGameplayStatics::ProjectWorldToScreen
			(
				UGameplayStatics::GetPlayerController(GetWorld(),0),
				FilterActor->GetActorLocation(),
				InScreenPosition
			);

			//视口大小
			FVector2D ViewportSize=UWidgetLayoutLibrary::GetViewportSize(GetWorld());

			//判断是否在屏幕视口里
			bool IsInViewport=
				InScreenPosition.X>0&&
				InScreenPosition.X<ViewportSize.X&&
				InScreenPosition.Y>0&&
				InScreenPosition.Y<ViewportSize.Y;

			//如果在视口里，添加此Actor和此Actor距离屏幕中心的XY大小
			if(IsInViewport)
			{
				InScreenActors.Add(FilterActor);
			}
		}
	}

	return InScreenActors;
}

AActor* ULockTargetComponent::GetClosestCenterScreenActor(const TArray<AActor*>& InActors)
{
	AActor* ResultActor=nullptr;
	
	if(InActors.Num()>0)
	{
		//记录在视口里的Actor距离屏幕中心X加上Y的大小，作为最近的依据
		TArray<float> ActorsInScreenXYSize;
		
		int32 MinIndex;
		float MinValue;
		
		for (AActor* FilterActor:InActors)
		{
			FVector2D InScreenPosition;
			
			//Actor在视口的位置
			UGameplayStatics::ProjectWorldToScreen
			(
				UGameplayStatics::GetPlayerController(GetWorld(),0),
				FilterActor->GetActorLocation(),
				InScreenPosition
			);

			//视口大小
			FVector2D ViewportSize=UWidgetLayoutLibrary::GetViewportSize(GetWorld());

			//屏幕中心位置
			FVector2D ScreenCenterPos(ViewportSize.X/2,ViewportSize.Y/2);
			
			//添加此Actor距离屏幕中心的XY大小
			//用绝对值来设置大小
			ActorsInScreenXYSize.Add(FMath::Abs(InScreenPosition.X-ScreenCenterPos.X)+FMath::Abs(InScreenPosition.Y-ScreenCenterPos.Y));
			
		}

		//得到距离最小的索引
		UKismetMathLibrary::MinOfFloatArray(ActorsInScreenXYSize,MinIndex,MinValue);

		if(MinIndex!=-1)
		{
			ResultActor=InActors[MinIndex];
		}
	}
	return ResultActor;
}

void ULockTargetComponent::UpdateCameraLockTarget()
{
	if (CurrentLockedTarget)
	{
		if(AController* ControllerToUpdate = OwnedCharacter->GetController())
		{
			FRotator TargetRotaion;
			TargetRotaion = UKismetMathLibrary::FindLookAtRotation(OwnedCharacter->GetActorLocation(),CurrentLockedTarget->GetActorLocation());
			//只锁定Yaw
			TargetRotaion.Pitch = ControllerToUpdate->GetControlRotation().Pitch;
			TargetRotaion.Roll = ControllerToUpdate->GetControlRotation().Roll;

			ControllerToUpdate->SetControlRotation(UKismetMathLibrary::RInterpTo(ControllerToUpdate->GetControlRotation(), TargetRotaion, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),RotateInterpSpeed));
		}
	}
}

void ULockTargetComponent::ClientBindLockEndDelegate_Implementation()
{
	BindLockEndDelegate();
}

void ULockTargetComponent::BindLockEndDelegate()
{
	if(CurrentLockedTarget)
	{
		if(ULockPosComponent* LockPosComponent=Cast<ULockPosComponent>(CurrentLockedTarget->GetComponentByClass(ULockPosComponent::StaticClass())))
		{
			if(!LockPosComponent->OnLockEnd.IsBoundToObject(this))
			{
				LockPosComponent->OnLockEnd.AddUObject(this, &ULockTargetComponent::HandleLockEnd);
			}
		}
	}
}

void ULockTargetComponent::ClientUnBindLockEndDelegate_Implementation()
{
	UnbindLockEndDelegate();
}

void ULockTargetComponent::UnbindLockEndDelegate()
{
	if(CurrentLockedTarget)
	{
		if(ULockPosComponent* LockPosComponent=Cast<ULockPosComponent>(CurrentLockedTarget->GetComponentByClass(ULockPosComponent::StaticClass())))
		{
			if(LockPosComponent->OnLockEnd.IsBoundToObject(this))
			{
				LockPosComponent->OnLockEnd.RemoveAll(this);
			}
		}
	}
}

void ULockTargetComponent::HandleLockEnd()
{
	CurrentLockedTarget = nullptr;
	OnLockStateChange.Broadcast(false);
}

void ULockTargetComponent::ServerSetActorToLock_Implementation(AActor* InActor)
{
	if(InActor)
	{
		if (CurrentLockedTarget)
		{
			if(CurrentLockedTarget!=InActor)
			{
				SetActorShowLockPosIcon(CurrentLockedTarget,false);
				CurrentLockedTarget=InActor;
				SetActorShowLockPosIcon(CurrentLockedTarget,true);
				OnLockStateChange.Broadcast(true);
			}
		}
		else
		{
			CurrentLockedTarget=InActor;
			SetActorShowLockPosIcon(CurrentLockedTarget,true);
			BindLockEndDelegate();
			ClientBindLockEndDelegate();
			OnLockStateChange.Broadcast(true);
		}
	}
}

void ULockTargetComponent::SetActorShowLockPosIcon_Implementation(AActor* InActor, bool InShow)
{
	if(InActor)
	{
		if(ULockPosComponent* LockPosComponent=Cast<ULockPosComponent>(InActor->GetComponentByClass(ULockPosComponent::StaticClass())))
		{
			LockPosComponent->SetShowLockPosIcon(InShow);
		}
	}
}

void ULockTargetComponent::ServerCheckCanLockTarget_Implementation()
{
	if(CurrentLockedTarget)
	{
		if(CurrentLockedTarget->GetDistanceTo(OwnedCharacter)>CheckRadius)
		{
			ServerUnLockCurrentTarget();
		}
	}
}

void ULockTargetComponent::ServerUnLockCurrentTarget_Implementation()
{
	if (CurrentLockedTarget)
	{
		SetActorShowLockPosIcon(CurrentLockedTarget, false);
	}
	UnbindLockEndDelegate();
	ClientUnBindLockEndDelegate();
	CurrentLockedTarget = nullptr;
	OnLockStateChange.Broadcast(false);
}



