// Fill out your copyright notice in the Description page of Project Settings.


#include "HitSwitchOpenDoor.h"
#include "Door.h"


AHitSwitchOpenDoor::AHitSwitchOpenDoor()
{
	SwitchTimerDuration = 1.f;

}

void AHitSwitchOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AHitSwitchOpenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHitSwitchOpenDoor::SwitchFunction()
{
	if (bCanToggle && bReady)
	{
		ToggleMeshMaterial();

		for (ADoor* Doors : DoorsToOpen)
		{
			if (!Doors->bLowered && !Doors->bMoving)
			{
				bReady = false;
				ReadyTimer();
				Doors->LowerDoor();
				bSwitchedOn = true;
				bRotate = false;
			}
			else if(Doors->bLowered && !Doors->bMoving)
			{
				bReady = false;
				ReadyTimer();
				Doors->RaiseDoor();
				bSwitchedOn = false;
				bRotate = true;
			}
		}
	}
	else if (!bSwitchedOn && !bCanToggle && bReady)
	{
		ToggleMeshMaterial();
		for (ADoor* Doors : DoorsToOpen)
		{
			if (!Doors->bLowered && !Doors->bMoving)
			{
				Doors->LowerDoor();
				bSwitchedOn = true;
				bRotate = false;
			}
		}
	}
}

void AHitSwitchOpenDoor::ToggleMeshMaterial()
{
	Super::ToggleMeshMaterial();
}

void AHitSwitchOpenDoor::ToggleReadyStatus()
{
	Super::ToggleReadyStatus();
}

void AHitSwitchOpenDoor::ReadyTimer()
{
	Super::ReadyTimer();
}
