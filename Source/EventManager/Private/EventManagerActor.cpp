// Fill out your copyright notice in the Description page of Project Settings.


#include "EventManagerActor.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AEventManagerActor::AEventManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AEventManagerActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEventManagerActor,LastCalledEventName);
}

void AEventManagerActor::Multicast_CallEvent_Implementation(FName Event)
{
	if(Event == NAME_None) return;
	LastCalledEventName = Event;
	if(Event.ToString().StartsWith(*EventManagerConst::CheckPointPrefix))
	{
		LastCheckpointEventName = Event;
	}
	OnEventCalledDelegate.Broadcast(LastCalledEventName);
	K2_OnCallEvent(Event);
}

void AEventManagerActor::CallEvent(FName Event)
{
	Multicast_CallEvent(Event);
}

void AEventManagerActor::ResetLastCheckpoint(float TimeToReset)
{
	if(LastCheckpointEventName == NAME_None) return;

	TWeakObjectPtr<AEventManagerActor> WeakThis{this};
	GetWorldTimerManager().SetTimer(ResetTimerHandle,[WeakThis]()
	{
		if(WeakThis->IsValidLowLevel() && WeakThis->HasAuthority())
		{
			WeakThis->CallEvent(WeakThis->LastCheckpointEventName);
		}
	},TimeToReset,false);
}
