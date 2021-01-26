// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonDoorwayLocked.h"
#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"


ADungeonDoorwayLocked::ADungeonDoorwayLocked()
{
	LockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockMesh"));
	LockMesh->SetupAttachment(GetRootComponent());
}

void ADungeonDoorwayLocked::BeginPlay()
{
	Super::BeginPlay();
}

void ADungeonDoorwayLocked::InteractFunction(AMain* Char)
{
	Super::InteractFunction(Char);
	if (InteractWidget)
	{
		InteractWidget->DestroyComponent();
	}
}

void ADungeonDoorwayLocked::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADungeonDoorwayLocked::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ADungeonDoorwayLocked::OpenDoor()
{
	Super::OpenDoor();
}

void ADungeonDoorwayLocked::UpdateDoorLocation(float Z)
{
	Super::UpdateDoorLocation(Z);
}

bool ADungeonDoorwayLocked::CheckConditions()
{
	Super::CheckConditions();
	if (MainCharacter->KeyCount >= 1)
	{
		MainCharacter->ChangeKeyCount(-1);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UnlockParticles, LockMesh->GetComponentToWorld().GetLocation(), FRotator(0.f), true);
		LockMesh->DestroyComponent();

		return true;
	}
	else
	{
		return false;
	}
}
