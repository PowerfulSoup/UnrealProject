// Fill out your copyright notice in the Description page of Project Settings.


#include "HitSwitchSpawnActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


AHitSwitchSpawnActor::AHitSwitchSpawnActor()
{
	SwitchTimerDuration = 1.f;
	bReady = true;
}

void AHitSwitchSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	ActorToShow->SetActorHiddenInGame(true);
	ActorToShow->SetActorEnableCollision(false);
}

void AHitSwitchSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHitSwitchSpawnActor::SwitchFunction()
{
	Super::SwitchFunction();

	if (ActorToShow && bReady)
	{
		ToggleMeshMaterial();
		ActorToShow->SetActorHiddenInGame(false);
		ActorToShow->SetActorEnableCollision(true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActorSpawnParticle, ActorToShow->GetActorLocation(), FRotator(0.f), true);
		bReady = false;
	}
}

void AHitSwitchSpawnActor::ToggleMeshMaterial()
{
	Super::ToggleMeshMaterial();
}

void AHitSwitchSpawnActor::ToggleReadyStatus()
{
	Super::ToggleReadyStatus();
}

void AHitSwitchSpawnActor::ReadyTimer()
{
	Super::ReadyTimer();
}
