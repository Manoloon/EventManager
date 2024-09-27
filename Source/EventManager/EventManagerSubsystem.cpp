// // Copyright Pablo Rodrigo Sanchez, Inc. All Rights Reserved.

#include "EventManagerSubsystem.h"
#include "Net/UnrealNetwork.h"

void UEventManagerSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEventManagerSubsystem, LastCalledEventName);
}

void UEventManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEventManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEventManagerSubsystem::Multicast_CallEvent_Implementation(FName Event)
{
	if(Event == NAME_None) return;

	LastCalledEventName = Event;
	if(Event.ToString().StartsWith(*EventManagerLabels::CheckpointPrefix))
	{
		LastCheckpointEventName = Event;
	}
	OnEventCalledDelegate.Broadcast(LastCalledEventName);
}

void UEventManagerSubsystem::CallEvent(FName Event)
{
	Multicast_CallEvent(Event);
}

void UEventManagerSubsystem::ResetLastCheckpoint(float TimeToReset)
{
	if(LastCheckpointEventName == NAME_None) return;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this,"CallEvent",LastCheckpointEventName);
	GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle,TimerDelegate,TimeToReset,false);
}