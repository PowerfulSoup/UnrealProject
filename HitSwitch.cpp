// Fill out your copyright notice in the Description page of Project Settings.


#include "HitSwitch.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon.h"
#include "Materials/Material.h"
#include "Engine/World.h"

// Sets default values
AHitSwitch::AHitSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target Mesh"));
	TargetMesh->SetCollisionProfileName(FName("Pawn"));
	TargetMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	TargetMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	TargetMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	RootComponent = TargetMesh;

	SwitchTimerDuration = 1.f;

	bSwitchedOn = false;
	bRotate = false;
	bCanToggle = false;
	bReady = true;
}

// Called when the game starts or when spawned
void AHitSwitch::BeginPlay()
{
	Super::BeginPlay();
	TargetMesh->SetMaterial(0, HitSwitchMaterialOff);

}

// Called every frame
void AHitSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * 45.f;
		SetActorRotation(Rotation);
	}
}

void AHitSwitch::SwitchFunction()
{

}

void AHitSwitch::ToggleMeshMaterial()
{
	if (bSwitchedOn)
	{
		TargetMesh->SetMaterial(0, HitSwitchMaterialOff);
	} 
	else
	{
		TargetMesh->SetMaterial(0, HitSwitchMaterialOn);
	}
}

void AHitSwitch::ReadyTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SwitchTimer, this, &AHitSwitch::ToggleReadyStatus, SwitchTimerDuration, false);
}

void AHitSwitch::ToggleReadyStatus()
{
	if (!bReady)
	{
		bReady = true;
	}
	else
	{
		bReady = false;
	}
}
