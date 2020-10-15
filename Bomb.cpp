// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"


ABomb::ABomb()
{

}

void ABomb::BeginPlay()
{
	Super::BeginPlay();


}

void ABomb::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void ABomb::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABomb::PrimaryFunction()
{

}

void ABomb::SecondaryFunction()
{

}

void ABomb::Equip(AMain* Char)
{
	Super::Equip(Char);

}

void ABomb::SetInstigator(AController* Inst)
{
	Super::SetInstigator(Inst);
}
