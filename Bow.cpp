// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Main.h"
#include "Arrow.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"



ABow::ABow()
{
	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	ArrowPlaceHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowPlaceHolder"));
	ArrowPlaceHolder->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ArrowPlaceHolder->SetupAttachment(SkeletalMesh);

	ToolName = "Bow";

	bDrawn = false;
	bReady = false;
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
	Super::PrimaryFunction();

	if (ToolOwner->ArrowAmmoCheck())
	{	
		if (bReady && ToolOwner->bRMBDown == false) //This got UPDATED FROM Lmb TO Rmb
		{
			Fire();
			ToolOwner->ArrowAmmo--;
		}
	}
}

void ABow::SecondaryFunction()//Zoom
{
	ToolOwner->ToggleZoomCamera();
}

void ABow::Fire()
{
	if (bReady)
	{
		FRotator Direction;
		if (ToolOwner->bHasCombatTarget)
		{
			AEnemy* EnemyTarget = ToolOwner->CombatTarget;
			Direction = UKismetMathLibrary::FindLookAtRotation(ToolOwner->GetActorLocation(), EnemyTarget->GetActorLocation());	
		}
		else
		{
			Direction = SkeletalMesh->GetSocketRotation("ArrowSpawnSocket");
		}

		ArrowPlaceHolder->SetHiddenInGame(true);
		FVector Location = SkeletalMesh->GetSocketLocation("ArrowSpawnSocket");
		FActorSpawnParameters SpawnInfo;

		AArrow* SpawnedArrow = GetWorld()->SpawnActor<AArrow>(ArrowToSpawn->GetDefaultObject()->GetClass(), Location, Direction, SpawnInfo);

		bReady = false;

		ToolOwner->PlayToolAnimation();
	}
}

void ABow::Equip(class AMain* Char)
{
	Super::Equip(Char);
	ToolOwner->SetToolType(EToolType::ETT_Bow);
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

