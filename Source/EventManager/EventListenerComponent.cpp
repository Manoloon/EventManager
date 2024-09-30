// Fill out your copyright notice in the Description page of Project Settings.


#include "EventListenerComponent.h"

#include "EventManagerSubsystem.h"


// Sets default values for this component's properties
UEventListenerComponent::UEventListenerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEventListenerComponent::BeginPlay()
{
	Super::BeginPlay();
	DefaultEventData = EventData;
	if (UEventManagerSubsystem* EventManager = GetWorld()->GetSubsystem<UEventManagerSubsystem>())
	{
		EventManager->OnEventCalledDelegate.AddUObject(this, &ThisClass::OnEventReceived);
	}
}

void UEventListenerComponent::OnEventReceived(FName Event)
{
	if(EventData.ActivationByEvent && EventData.ActivationEvent == Event)
	{
		ActivateByEvent();
	}
	if(EventData.DeactivationByEvent && EventData.DeactivationEvent == Event)
	{
		DeactivateByEvent();
	}
	if(Event == *EventManagerLabels::ResetEventPrefix)
	{
		ResetEventCalled();
	}
	OnReceivedEventProcessed(Event);
}

void UEventListenerComponent::OnReceivedEventProcessed(FName Event)
{
	K2_OnReceivedEventProcessed(Event);
}

void UEventListenerComponent::ActivateByEvent()
{
	if(Activated) return;
	Activated = true;
	K2_OnActivateByEvent();
}

void UEventListenerComponent::DeactivateByEvent()
{
	K2_OnDeactivateByEvent();
}

void UEventListenerComponent::ResetEventCalled()
{
	if(DeniedReset || DefaultEventData.ActivationByEvent) return;

	EventData = DefaultEventData;
	// TODO : Should be false straight
	Activated = Activated && DeniedReset;
	K2_OnResetEventCalled();
	DeactivateByEvent();
}


// Called every frame
void UEventListenerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEventListenerComponent::CallEvent(FName Event)
{
	if(Event == NAME_None) return;
	if(UEventManagerSubsystem* EventManager = GetWorld()->GetSubsystem<UEventManagerSubsystem>())
	{
		EventManager->CallEvent(Event);
	}
}

