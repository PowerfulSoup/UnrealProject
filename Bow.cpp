// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Main.h"
#include "Arrow.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"



ABow::ABow()
{
	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	ArrowPlaceHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowPlaceHolder"));
	ArrowPlaceHolder->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ArrowPlaceHolder->SetupAttachment(SkeletalMesh);

	ToolName = "Bow";

	bDrawn = false;
	bIsEquippedInRightHand = false;
}

void ABow::BeginPlay()
{
	Super::BeginPlay();
	InitialArrowLocation = ArrowPlaceHolder->GetComponentLocation();
	ArrowPlaceHolder->SetHiddenInGame(true);

	TrueInitialArrowPHLocation = InitialArrowLocation;

}

void ABow::PrimaryFunction()//Draw/Fire
{
	if (bDrawn && ToolOwner->bLMBDown == false)
	{
		Fire();
	}
}

void ABow::SecondaryFunction()//Zoom
{

	ToolOwner->ToggleZoomCamera();
}

void ABow::Fire()
{
	if (bDrawn)
	{
		ArrowPlaceHolder->SetHiddenInGame(true);

		FRotator Direction = SkeletalMesh->GetSocketRotation("ArrowSpawnSocket");
		FVector Location = SkeletalMesh->GetSocketLocation("ArrowSpawnSocket");
		FActorSpawnParameters SpawnInfo;

		AArrow* SpawnedArrow = GetWorld()->SpawnActor<AArrow>(ArrowToSpawn->GetDefaultObject()->GetClass(), Location, Direction, SpawnInfo);

		bDrawn = false;

	}
}

void ABow::Equip(class AMain* Char)
{
	Super::Equip(Char);
}

void ABow::SetInstigator(AController* Inst)
{
	Super::SetInstigator(Inst);
}

void ABow::SetToolOwner(AMain* Char)
{
	Super::SetToolOwner(Char);
}

void ABow::DrawArrowPlaceHolder(float X)
	{
	ArrowPlaceHolder->SetHiddenInGame(false);

	FVector NewLocation = InitialArrowLocation;
	NewLocation.Y += X;
	ArrowPlaceHolder->SetRelativeLocation(NewLocation);
	}
