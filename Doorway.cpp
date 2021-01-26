// Fill out your copyright notice in the Description page of Project Settings.


#include "Doorway.h"

ADoorway::ADoorway()
{


}

void ADoorway::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorway::InteractFunction(AMain* Char)
{
	Super::InteractFunction(Char);
	OpenDoor();
}

void ADoorway::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoorway::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ADoorway::OpenDoor()
{

}
