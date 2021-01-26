// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonDoorway.h"
#include "Main.h"

ADungeonDoorway::ADungeonDoorway()
{

}

void ADungeonDoorway::BeginPlay()
{
	Super::BeginPlay();

	InitialDoorLocation = InteractableMesh->GetComponentLocation();
}

void ADungeonDoorway::InteractFunction(AMain* Char)
{
	MainCharacter = Char;
	Super::InteractFunction(Char);
}

void ADungeonDoorway::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADungeonDoorway::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (MainCharacter)
	{
		MainCharacter = nullptr;
	}
}

void ADungeonDoorway::OpenDoor()
{
	Super::OpenDoor();

	if (CheckConditions())
	{
		LowerDoor();
	}
}

void ADungeonDoorway::UpdateDoorLocation(float Z)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += Z;
	InteractableMesh->SetWorldLocation(NewLocation);
}

bool ADungeonDoorway::CheckConditions()
{
	return true;
}
