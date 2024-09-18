// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventManagerActor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventCalled,FName);

namespace EventManagerConst
{
	static const FString CheckPointPrefix = TEXT("CKP_");
	static const FString ResetEventPrefix = TEXT("EVR_");
}

UCLASS(meta = (DisableNativeTick))
class EVENTMANAGER_API AEventManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventManagerActor();
	FOnEventCalled OnEventCalledDelegate;
	UFUNCTION(BlueprintCallable)
	void CallEvent(FName Event);
	UFUNCTION(BlueprintCallable)
	virtual void ResetLastCheckpoint(float TimeToReset) final;
protected:
	UPROPERTY(EditDefaultsOnly)
	float ResetTime = 2.f;
	UPROPERTY(Replicated)
	FName LastCalledEventName = NAME_None;
	FName LastCheckpointEventName = NAME_None;
	
	FTimerHandle ResetTimerHandle;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_CallEvent(FName Event);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnCallEvent(FName Event);
	
};
