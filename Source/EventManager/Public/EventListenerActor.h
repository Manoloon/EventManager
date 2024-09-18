// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventListenerActor.generated.h"

class AEventManagerActor;

USTRUCT(BlueprintType)
struct FEventListenerData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool ActivationByEvent = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ActivationByEvent"))
	FName ActivationEvent = NAME_None;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool DeactivationByEvent = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DeactivationByEvent"))
	FName DeactivationEvent  = NAME_None;
};

UCLASS(Abstract, ClassGroup = EventListener, meta = (DisableNativeTick))
class EVENTMANAGER_API AEventListenerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventListenerActor();
	UFUNCTION(BlueprintCallable)
	void CallEvent(FName Event);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,Category = Settings)
	FEventListenerData EventData;
	FEventListenerData DefaultEventData;
	bool Activated = false;
	UPROPERTY(EditAnywhere, Category = Settings)
	bool DeniedReset = false;

	UFUNCTION()
	void OnEventReceived(FName Event);
	
	void OnReceivedEventProcessed(FName Event);
	UFUNCTION(BlueprintImplementableEvent,meta =( DisplayName = "OnReceivedEventProcessed", ScriptName = "OnReceivedEventProcessed"))
	void K2_OnReceivedEventProcessed(FName Event);

	void ActivateByEvent();
	UFUNCTION(BlueprintImplementableEvent,meta =( DisplayName = "OnActivateByEvent", ScriptName = "OnActivateByEvent"))
	void K2_OnActivateByEvent();

	void DeactivateByEvent();
	UFUNCTION(BlueprintImplementableEvent,meta =( DisplayName = "OnDeactivateByEvent", ScriptName = "OnDeactivateByEvent"))
	void K2_OnDeactivateByEvent();

	void ResetEventCalled();
	UFUNCTION(BlueprintImplementableEvent,meta =( DisplayName = "OnResetEventCalled", ScriptName = "OnResetEventCalled"))
	void K2_OnResetEventCalled();

	AEventManagerActor* GetEventManager() const;
};
