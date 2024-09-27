// // Copyright Pablo Rodrigo Sanchez, Inc. All Rights Reserved.

#include "..\Public\EventListenerActor.h"
#include "EventManager/EventManagerSubsystem.h"

AEventListenerActor::AEventListenerActor()
{
	bReplicates = true;
}

// Called when the game starts or when spawned
void AEventListenerActor::BeginPlay()
{
	Super::BeginPlay();
	DefaultEventData = EventData;
	//AEventManagerActor* EventManager = GetEventManager();
	if (UEventManagerSubsystem* EventManager = GetWorld()->GetSubsystem<UEventManagerSubsystem>())
	{
		EventManager->OnEventCalledDelegate.AddUObject(this, &ThisClass::OnEventReceived);
	}
}

void AEventListenerActor::OnEventReceived(FName Event)
{
	UE_LOG(LogTemp, Warning, TEXT("EVent REceived %s"), *Event.ToString());
	if (EventData.ActivationByEvent && EventData.ActivationEvent == Event)
	{
		ActivateByEvent();
	}
	if (EventData.DeactivationByEvent && EventData.DeactivationEvent == Event)
	{
		DeactivateByEvent();
	}
	if (Event == *EventManagerLabels::ResetEventPrefix)
	{
		ResetEventCalled();
	}
	OnReceivedEventProcessed(Event);
}

void AEventListenerActor::CallEvent(FName Event)
{
	if (Event == NAME_None)
		return;
	
	if (UEventManagerSubsystem* EventManager = GetWorld()->GetSubsystem<UEventManagerSubsystem>())
	{
		EventManager->CallEvent(Event);
	}
}

void AEventListenerActor::ActivateByEvent()
{
	if (Activated)
		return;

	Activated = true;
	K2_OnActivateByEvent();
}

void AEventListenerActor::OnReceivedEventProcessed(FName Event)
{
	K2_OnReceivedEventProcessed(Event);
}

void AEventListenerActor::DeactivateByEvent()
{
	K2_OnDeactivateByEvent();
}

void AEventListenerActor::ResetEventCalled()
{
	if (DeniedReset || DefaultEventData.ActivationByEvent)
		return;

	EventData = DefaultEventData;
	Activated = Activated && DeniedReset;
	K2_OnResetEventCalled();
	DeactivateByEvent();
}
