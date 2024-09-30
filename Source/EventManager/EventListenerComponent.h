// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EventListenerComponent.generated.h"

USTRUCT(BlueprintType)
struct FEventListenerComData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool ActivationByEvent = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ActivationByEvent"))
	FName ActivationEvent = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool DeactivationByEvent = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DeactivationByEvent"))
	FName DeactivationEvent = NAME_None;
};

UCLASS(Abstract,Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent,DisableNativeTick))
class EVENTMANAGER_API UEventListenerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEventListenerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Settings)
	FEventListenerComData EventData;
	FEventListenerComData DefaultEventData;
	bool Activated = false;
	UPROPERTY(EditAnywhere, Category = Settings)
	bool DeniedReset = false;

	UFUNCTION()
	void OnEventReceived(FName Event);

	void OnReceivedEventProcessed(FName Event);
	UFUNCTION(BlueprintImplementableEvent, meta =( DisplayName = "OnReceivedEventProcessed", ScriptName = "OnReceivedEventProcessed"))
	void K2_OnReceivedEventProcessed(FName Event);

	void ActivateByEvent();
	UFUNCTION(BlueprintImplementableEvent, meta =( DisplayName = "OnActivateByEvent", ScriptName = "OnActivateByEvent"))
	void K2_OnActivateByEvent();

	void DeactivateByEvent();
	UFUNCTION(BlueprintImplementableEvent, meta =( DisplayName = "OnDeactivateByEvent", ScriptName = "OnDeactivateByEvent"))
	void K2_OnDeactivateByEvent();

	void ResetEventCalled();
	UFUNCTION(BlueprintImplementableEvent, meta =( DisplayName = "OnResetEventCalled", ScriptName = "OnResetEventCalled"))
	void K2_OnResetEventCalled();
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CallEvent(FName Event);

};