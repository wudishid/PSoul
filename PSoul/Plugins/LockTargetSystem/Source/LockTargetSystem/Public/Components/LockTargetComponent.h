// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockTargetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockStateChange,bool,IsLock);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOCKTARGETSYSTEM_API ULockTargetComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	ULockTargetComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentLockTarget();
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerLockTarget();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerCancelLockTarget();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	//向左方向切换锁定目标
	void ServerLeftSwitchLockTarget();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	//向左方向切换锁定目标
	void ServerRightSwitchLockTarget();

	bool IsLockingTarget() const;

	UPROPERTY(BlueprintAssignable)
	FOnLockStateChange OnLockStateChange;

	FORCEINLINE AActor* GetCurrentLockTarget() const{return CurrentLockedTarget;}
private:
	//找到能被锁定的所有目标
	TArray<AActor*> FindCanBeLockedTargets();

	UFUNCTION(Client, Reliable)
	void ClientFilterActorsAndLock(const TArray<AActor*>& InActors);

	UFUNCTION(Client, Reliable)
	void ClientLeftSwitchLockTarget(const TArray<AActor*>& InActors);

	UFUNCTION(Client, Reliable)
	void ClientRightSwitchLockTarget(const TArray<AActor*>& InActors);
	
	TArray<AActor*> GetInScreenActors(const TArray<AActor*>& InActors);
	
	//得到离屏幕中心最近的Actor
	AActor* GetClosestCenterScreenActor(const TArray<AActor*>& InActors);
	
	//使摄像机一直锁定目标
	void UpdateCameraLockTarget();

	//设置要被锁定的Actor
	UFUNCTION(Server, Reliable)
	void ServerSetActorToLock(AActor* InActor);

	//设置Actor显示锁定图标
	UFUNCTION(Client, Reliable)
	void SetActorShowLockPosIcon(AActor* InActor,bool InShow);

	UFUNCTION(Server, Reliable)
	void ServerCheckCanLockTarget();

	UFUNCTION(Server, Reliable)
	void ServerUnLockCurrentTarget();

	UFUNCTION(Client, Reliable)
	void ClientBindLockEndDelegate();

	void BindLockEndDelegate();

	UFUNCTION(Client, Reliable)
	void ClientUnBindLockEndDelegate();
	
	void UnbindLockEndDelegate();
	
	void HandleLockEnd();
private:
	UPROPERTY(EditDefaultsOnly,Category="LockTarget",meta=(UIMin=0,ClampMin=0))
	float CheckRadius=3000;

	UPROPERTY(EditDefaultsOnly,Category="LockTarget",meta=(UIMin=0,ClampMin=0))
	float RotateInterpSpeed=5;
	
	//当前锁定的目标
	UPROPERTY(ReplicatedUsing = OnRep_CurrentLockTarget)
	TObjectPtr<AActor> CurrentLockedTarget=nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> OwnedCharacter=nullptr;
	
};
