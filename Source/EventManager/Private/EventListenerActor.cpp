// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\EventListenerActor.h"

#include "EventManagerActor.h"
#include "Kismet/GameplayStatics.h"

AEventListenerActor::AEventListenerActor()
{
	bReplicates = true;
}

// Called when the game starts or when spawned
void AEventListenerActor::BeginPlay()
{
	Super::BeginPlay();
	DefaultEventData = EventData;
	AEventManagerActor* EventManager = GetEventManager();
	if(EventManager != nullptr)
	{
		EventManager->OnEventCalledDelegate.AddUObject(this,&ThisClass::OnEventReceived);
	}
}

void AEventListenerActor::OnEventReceived(FName Event)
{
	UE_LOG(LogTemp,Warning,TEXT("EVent REceived %s"),*Event.ToString());
	if (EventData.ActivationByEvent && EventData.ActivationEvent == Event)
	{
		ActivateByEvent();
	}
	if (EventData.DeactivationByEvent && EventData.DeactivationEvent == Event)
	{
		DeactivateByEvent();
	}
	if (Event == *EventManagerConst::ResetEventPrefix)
	{
		ResetEventCalled();
	}
	OnReceivedEventProcessed(Event);
}

void AEventListenerActor::CallEvent(FName Event)
{	
	if (Event == NAME_None)
		return;
	AEventManagerActor* EventManager = GetEventManager();
	if (EventManager != nullptr)
	{
		EventManager->CallEvent(Event);
	}
	else
	{
		EventManager = GetEventManager();
		if(EventManager != nullptr)
		{
			EventManager->CallEvent(Event);
		}
	}
}

void AEventListenerActor::ActivateByEvent()
{
	if (Activated) return;

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
	if (DeniedReset || DefaultEventData.ActivationByEvent) return;

	EventData = DefaultEventData;
	Activated = Activated && DeniedReset;
	K2_OnResetEventCalled();
	DeactivateByEvent();
}

AEventManagerActor* AEventListenerActor::GetEventManager() const
{
	AActor* Actor = UGameplayStatics::GetActorOfClass(this, AEventManagerActor::StaticClass());
	return Cast<AEventManagerActor>(Actor);
}
