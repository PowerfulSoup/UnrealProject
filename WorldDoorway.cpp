// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDoorway.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"


AWorldDoorway::AWorldDoorway()
{

}

void AWorldDoorway::BeginPlay()
{
	Super::BeginPlay();
	InitialDoorRotation = InteractableMesh->GetComponentRotation();
}

void AWorldDoorway::InteractFunction(AMain* Char)
{
	Super::InteractFunction(Char);
}

void AWorldDoorway::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWorldDoorway::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AWorldDoorway::OpenDoor()
{
	Super::OpenDoor();
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwingOpen();
	GetWorld()->GetTimerManager().SetTimer(CloseTimer, this, &AWorldDoorway::SwingClosed, 4.f, false);

}

void AWorldDoorway::UpdateDoorRotation(float Yaw)
{
	FRotator NewRotation = InitialDoorRotation;
	NewRotation.Yaw += Yaw;
	InteractableMesh->SetWorldRotation(NewRotation);
}