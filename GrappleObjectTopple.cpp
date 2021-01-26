// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleObjectTopple.h"


AGrappleObjectTopple::AGrappleObjectTopple()
{

}

void AGrappleObjectTopple::BeginPlay()
{
	Super::BeginPlay();
}

void AGrappleObjectTopple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrappleObjectTopple::GrappleObjectFuntion()
{
	Super::GrappleObjectFuntion();
	if (!bActivated)
	{
		ToppleStart();
	}
}

void AGrappleObjectTopple::Topple(float Roll)
{
	FRotator NewRotation = InitialRotation;
	NewRotation.Roll += Roll;
	ObjectMesh->SetWorldRotation(NewRotation);
}