// Fill out your copyright notice in the Description page of Project Settings.


#include "HitSwitchOpenDoor.h"
#include "DungeonDoorway.h"
//#include "Door.h"


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

		for (ADungeonDoorway* DungeonDoorways : DungeonDoorwaysToOpen)
		{
			if (!DungeonDoorways->bLowered && !DungeonDoorways->bMoving)
			{
				bReady = false;
				ReadyTimer();
				DungeonDoorways->LowerDoor();
				bSwitchedOn = true;
				bRotate = false;
			}
			else if (DungeonDoorways->bLowered && !DungeonDoorways->bMoving)
			{
				bReady = false;
				ReadyTimer();
				DungeonDoorways->RaiseDoor();
				bSwitchedOn = false;
				bRotate = true;
			}
		}
	}
	else if (!bSwitchedOn && !bCanToggle && bReady)
	{
		ToggleMeshMaterial();
		for (ADungeonDoorway* DungeonDoorways : DungeonDoorwaysToOpen)
		{
			if (!DungeonDoorways->bLowered && !DungeonDoorways->bMoving)
			{
				DungeonDoorways->LowerDoor();
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
